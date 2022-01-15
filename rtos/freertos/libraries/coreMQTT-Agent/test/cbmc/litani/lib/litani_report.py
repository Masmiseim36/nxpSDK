# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License").
# You may not use this file except in compliance with the License.
# A copy of the License is located at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# or in the "license" file accompanying this file. This file is distributed
# on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
# express or implied. See the License for the specific language governing
# permissions and limitations under the License.


import datetime
import enum
import functools
import json
import logging
import os
import pathlib
import re
import shutil
import subprocess
import sys
import tempfile
import uuid

import jinja2

from lib import litani


def get_run(cache_dir):
    with open(cache_dir / litani.CACHE_FILE) as handle:
        ret = json.load(handle)
    ret["pipelines"] = {}

    for job in ret["jobs"]:
        status_file = litani.get_status_dir() / ("%s.json" % job["job_id"])
        try:
            with open(str(status_file)) as handle:
                status = json.load(handle)
        except FileNotFoundError:
            status = {
                "complete": False,
                "wrapper_arguments": job,
            }

        pipeline_name = status["wrapper_arguments"]["pipeline_name"]
        ci_stage = status["wrapper_arguments"]["ci_stage"]

        try:
            ret["pipelines"][pipeline_name]["ci_stages"][ci_stage]["jobs"].append(status)
        except KeyError:
            try:
                ret["pipelines"][pipeline_name]["ci_stages"][ci_stage]["jobs"] = [status]
            except KeyError:
                try:
                    ret["pipelines"][pipeline_name]["ci_stages"][ci_stage] = {
                        "jobs": [status]
                    }
                except KeyError:
                    try:
                        ret["pipelines"][pipeline_name]["ci_stages"] = {
                            ci_stage: {
                                "name": ci_stage,
                                "jobs": [status],
                            }
                        }
                    except KeyError:
                        ret["pipelines"][pipeline_name] = {
                            "name": pipeline_name,
                            "ci_stages":  {
                                ci_stage: {
                                    "jobs": [status]
                                }
                            },
                        }
    ret.pop("jobs")
    return ret


def job_sorter(j1, j2):
    if j1 is None or j2 is None:
        raise ValueError("Jobs must be non-None")
    if not ("start_time" in j1 or "start_time" in j2):
        return 0
    if not "start_time" in j1:
        return 1
    if not "start_time" in j2:
        return -1
    if j1["start_time"] != j2["start_time"]:
        return -1 if j1["start_time"] < j2["start_time"] else 1
    if not ("end_time" in j1 or "end_time" in j2):
        return 0
    if not "end_time" in j1:
        return 1
    if not "end_time" in j2:
        return -1
    return -1 if j1["end_time"] < j2["end_time"] else 1


class StageStatus(enum.IntEnum):
    FAIL = 0
    FAIL_IGNORED = 1
    SUCCESS = 2


def add_stage_stats(stage, stage_name, pipeline_name):
    n_complete_jobs = len([j for j in stage["jobs"] if j["complete"]])
    if len(stage["jobs"]):
        stage["progress"] = int(n_complete_jobs * 100 / len(stage["jobs"]))
        stage["complete"] = n_complete_jobs == len(stage["jobs"])
    else:
        stage["progress"] = 0
        stage["complete"] = True
    status = StageStatus.SUCCESS
    for job in stage["jobs"]:
        try:
            if not job["complete"]:
                continue
            elif job["wrapper_return_code"]:
                status = StageStatus.FAIL
            elif job["command_return_code"] and status == StageStatus.SUCCESS:
                status = StageStatus.FAIL_IGNORED
            elif job["timeout_reached"] and status == StageStatus.SUCCESS:
                status = StageStatus.FAIL_IGNORED
        except KeyError:
            logging.error(
                "Could not find key in stage: %s",
                json.dumps(stage, indent=2))
            sys.exit(1)
    stage["status"] = status.name.lower()
    stage["url"] = "artifacts/%s/%s/index.html" % (pipeline_name, stage_name)
    stage["name"] = stage_name


class PipeStatus(enum.IntEnum):
    FAIL = 0
    IN_PROGRESS = 1
    SUCCESS = 2


def add_pipe_stats(pipe):
    pipe["url"] = "pipelines/%s/index.html" % pipe["name"]
    incomplete = [s for s in pipe["ci_stages"] if not s["complete"]]
    if incomplete:
        pipe["status"] = PipeStatus.IN_PROGRESS
    else:
        pipe["status"] = PipeStatus.SUCCESS
    for stage in pipe["ci_stages"]:
        if stage["status"] in ["fail", "fail_ignored"]:
            pipe["status"] = PipeStatus.FAIL
            break


def add_run_stats(run):
    status = PipeStatus.SUCCESS
    if [p for p in run["pipelines"] if p["status"] == PipeStatus.IN_PROGRESS]:
        status = PipeStatus.IN_PROGRESS
    if [p for p in run["pipelines"] if p["status"] == PipeStatus.FAIL]:
        status = PipeStatus.FAIL
    run["status"] = status.name.lower()
    for pipe in run["pipelines"]:
        pipe["status"] = pipe["status"].name.lower()


def add_job_stats(jobs):
    for job in jobs:
        if not ("start_time" in job and "end_time" in job):
            job["duration_str"] = None
        else:
            s = datetime.datetime.strptime(
                job["start_time"], litani.TIME_FORMAT_R)
            e = datetime.datetime.strptime(
                job["end_time"], litani.TIME_FORMAT_R)
            seconds = (e - s).seconds
            job["duration_str"] = s_to_hhmmss(seconds)
            job["duration"] = seconds


def sort_run(run):
    pipelines = []
    js = functools.cmp_to_key(job_sorter)
    for pipe in run["pipelines"].values():
        stages = []
        for stage in litani.CI_STAGES:
            try:
                pipeline_stage = pipe["ci_stages"][stage]
            except KeyError:
                pipe["ci_stages"][stage] = {"jobs": []}
                pipeline_stage = pipe["ci_stages"][stage]
            jobs = sorted(pipeline_stage["jobs"], key=js)
            add_job_stats(jobs)
            pipeline_stage["jobs"] = jobs
            add_stage_stats(pipeline_stage, stage, pipe["name"])
            stages.append(pipeline_stage)

        pipe["ci_stages"] = stages
        add_pipe_stats(pipe)
        pipelines.append(pipe)
    pipelines = sorted(pipelines, key=lambda p: p["status"])
    run["pipelines"] = pipelines
    add_run_stats(run)


def get_run_data(cache_dir):
    run = get_run(cache_dir)
    sort_run(run)
    return run


def s_to_hhmmss(s):
    h, s = divmod(s, 3600)
    m, s = divmod(s, 60)
    if h:
        return "{h:02d}h {m:02d}m {s:02d}s".format(h=h, m=m, s=s)
    if m:
        return "{m:02d}m {s:02d}s".format(m=m, s=s)
    return "{s:02d}s".format(s=s)


def get_stats_groups(run):
    ret = {}
    for pipe in run["pipelines"]:
        for stage in pipe["ci_stages"]:
            for job in stage["jobs"]:
                if "tags" not in job["wrapper_arguments"]:
                    continue
                if not job["wrapper_arguments"]["tags"]:
                    continue
                stats_group = None
                for tag in job["wrapper_arguments"]["tags"]:
                    kv = tag.split(":")
                    if kv[0] != "stats-group":
                        continue
                    stats_group = kv[1]
                if not stats_group:
                    continue

                if "duration" not in job:
                    continue
                record = {
                    "pipeline": job["wrapper_arguments"]["pipeline_name"],
                    "duration": job["duration"]
                }
                try:
                    ret[stats_group].append(record)
                except KeyError:
                    ret[stats_group] = [record]
    return sorted([(k, v) for k, v in ret.items()])


def to_id(string):
    allowed = re.compile(r"[-a-zA-Z0-9\.]")
    return "".join([c if allowed.match(c) else "_" for c in string])


def render_runtimes(run, env, report_dir):
    stats_groups = get_stats_groups(run)
    svgs = []
    gnu_templ = env.get_template("runtime-box.jinja.gnu")
    img_dir = report_dir / "runtimes"
    img_dir.mkdir(exist_ok=True, parents=True)
    for group_name, jobs in stats_groups:
        if len(jobs) < 2:
            continue
        group_id = to_id(group_name)
        url = img_dir / ("%s.svg" % group_id)
        tmp_url = "%s~" % str(url)
        gnu_file = gnu_templ.render(
            group_name=group_name, jobs=jobs, url=tmp_url)
        with tempfile.NamedTemporaryFile("w") as tmp:
            print(gnu_file, file=tmp)
            tmp.flush()
            cmd = ["gnuplot", tmp.name]
            subprocess.run(
                cmd, check=True, stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL)
        os.rename(tmp_url, url)
        with open(url) as handle:
            lines = [
                l for l in handle.read().splitlines()
                if "<?xml version" not in l
            ]
        svgs.append(lines)
    return svgs


def get_git_hash():
    try:
        cmd = ["git", "show", "--no-patch", "--pretty=format:%h"]
        litani_dir = pathlib.Path(os.path.realpath(__file__)).parent
        proc = subprocess.run(
            cmd, text=True, stdout=subprocess.PIPE, check=True, cwd=litani_dir)
        return proc.stdout.strip()
    except RuntimeError:
        return None


def render(run, report_dir):
    temporary_report_dir = litani.get_report_data_dir() / str(uuid.uuid4())
    temporary_report_dir.mkdir(parents=True)
    old_report_dir_path = litani.get_report_dir().resolve()
    old_report_dir = litani.ExpireableDirectory(old_report_dir_path)

    artifact_dir = temporary_report_dir / "artifacts"
    shutil.copytree(litani.get_artifacts_dir(), artifact_dir)

    render_artifact_indexes(artifact_dir)

    template_dir = pathlib.Path(__file__).parent.parent / "templates"
    env = jinja2.Environment(loader=jinja2.FileSystemLoader(str(template_dir)))

    svgs = render_runtimes(run, env, temporary_report_dir)

    litani_report_archive_path = os.getenv("LITANI_REPORT_ARCHIVE_PATH")

    dash_templ = env.get_template("dashboard.jinja.html")
    page = dash_templ.render(
        run=run, svgs=svgs, litani_hash=get_git_hash(),
        litani_version=litani.VERSION,
        litani_report_archive_path=litani_report_archive_path)
    with litani.atomic_write(temporary_report_dir / "index.html") as handle:
        print(page, file=handle)

    with litani.atomic_write(temporary_report_dir / litani.RUN_FILE) as handle:
        print(json.dumps(run, indent=2), file=handle)

    pipe_templ = env.get_template("pipeline.jinja.html")
    for pipe in run["pipelines"]:
        page = pipe_templ.render(run=run, pipe=pipe)
        with litani.atomic_write(temporary_report_dir / pipe["url"]) as handle:
            print(page, file=handle)

    temp_symlink_dir = report_dir.with_name(report_dir.name + str(uuid.uuid4()))
    os.symlink(temporary_report_dir, temp_symlink_dir)
    os.rename(temp_symlink_dir, report_dir)

    # Release lock so that other processes can read from this directory
    new_report_dir = litani.LockableDirectory(report_dir.resolve())
    new_report_dir.release()

    if old_report_dir_path.exists():
        old_report_dir.expire()
    litani.unlink_expired()


def render_artifact_indexes(artifact_dir):
    def dirs_needing_indexes():
        for root, dirs, fyles in os.walk(artifact_dir):
            if "index.html" not in fyles:
                yield pathlib.Path(root), dirs, fyles

    template_dir = pathlib.Path(__file__).parent.parent / "templates"
    env = jinja2.Environment(loader=jinja2.FileSystemLoader(str(template_dir)))
    index_templ = env.get_template("file-list.jinja.html")
    for dyr, dirs, files in dirs_needing_indexes():
        page = index_templ.render(
            file_list=sorted(files), dir_list=sorted(dirs), root=str(dyr))
        with litani.atomic_write(dyr / "index.html") as handle:
            print(page, file=handle)
