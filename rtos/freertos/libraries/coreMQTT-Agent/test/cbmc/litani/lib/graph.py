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


import dataclasses
import os
import pathlib

import lib.litani
import lib.litani_report



class Node:
    pass



class DependencyNode(Node):
    def __init__(self, fyle):
        self.file = fyle
        self.id = hash(fyle)

        path = pathlib.Path(fyle)
        _, ext = os.path.splitext(path.name)

        if len(path.name) + len(ext) > 20:
            self.label = path.name[:(19 - len(ext))] + "…" + ext
        else:
            self.label = path.name


    def __hash__(self):
        return self.id


    def __eq__(self, other):
        return self.id == other.id


    def __str__(self):
        return '"{id}" [label="{label}"];'.format(
            id=self.id, label=self.label)



class CommandNode(Node):
    def __init__(self, command):
        self.command = command
        self.id = hash(command)

        if len(command) > 15:
            self.label = command[:14] + "…"
        else:
            self.label = command


    def __hash__(self):
        return self.id


    def __eq__(self, other):
        return self.id == other.id


    def __str__(self):
        return '"{id}" [shape=box, label="{label}"];'.format(
            id=self.id, label=self.label)



@dataclasses.dataclass
class Edge:
    src: Node
    dst: Node

    def __eq__(self, other):
        return all((self.src == other.src, self.dst == other.dst))


    def __hash__(self):
        return hash(self.src) + hash(self.dst)


    def __str__(self):
        return '"{src}" -> "{dst}" [style=bold];'.format(
            src=self.src.id, dst=self.dst.id)



@dataclasses.dataclass
class PipelineChooser:
    pipelines: list


    def should_skip(self, pipeline):
        return self.pipelines and not pipeline in self.pipelines



@dataclasses.dataclass
class Graph:
    run: dict
    pipeline_chooser: PipelineChooser


    def iter_jobs(self):
        for pipe in self.run["pipelines"]:
            if self.pipeline_chooser.should_skip(pipe["name"]):
                continue
            for stage in pipe["ci_stages"]:
                for job in stage["jobs"]:
                    yield job


    def __str__(self):
        buf = ["digraph G {"]

        nodes = set()
        edges = set()

        for job in self.iter_jobs():
            args = job["wrapper_arguments"]

            cmd_node = CommandNode(args["command"])
            nodes.add(cmd_node)
            if args["outputs"]:
                for output in args["outputs"]:
                    out_node = DependencyNode(output)
                    nodes.add(out_node)
                    edges.add(Edge(src=cmd_node, dst=out_node))
            if args["inputs"]:
                for inputt in args["inputs"]:
                    in_node = DependencyNode(inputt)
                    nodes.add(in_node)
                    edges.add(Edge(src=in_node, dst=cmd_node))

        buf.extend([("  %s" % str(n)) for n in nodes])
        buf.extend([("  %s" % str(e)) for e in edges])
        buf.append("}")
        return "\n".join(buf)



def print_graph(args):
    lib.litani.add_jobs_to_cache()
    run = lib.litani_report.get_run_data(lib.litani.get_cache_dir())

    pc = PipelineChooser(args.pipelines)

    graph = Graph(run=run, pipeline_chooser=pc)
    print(graph)
