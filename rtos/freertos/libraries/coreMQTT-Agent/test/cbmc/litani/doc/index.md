
<header id="title">
  <h1>AWS Build Accumulator</h1>
  <p id="subtitle">Litani Command-line reference</p>
</header><!-- id="title" -->

AWS Build Accumulator is a suite of tools that collect build jobs from multiple sources before executing them
concurrently.
Users access AWS Build Accumulator through `litani`, the command-line
interface.
This document serves as a reference for using `litani` and integrating
it into your project.


`litani` allows you to use multiple build systems in the same project,
providing a backend that each build system emits jobs to.
Once all jobs have been enqueued, `litani` executes them all as a single
unified build graph.
`litani` also provides platform-independent job control (timeouts and
control of return codes), as well as grouping of job artifacts into
stages.

[Source code repository](https://github.com/awslabs/aws-build-accumulator)


Overview
--------

Consider the following Makefile:

    foo.out: foo.in
        timeout 90 my_command $^ > $@; if [ $? -eq 10 ]; then exit 0; fi

`foo.out` is built from `foo.in` using `my_command`. We want to kill
`my_command` if it runs for more than 90 seconds, and we're also
expecting that `my_command` may exit with a return code of 10; we don't
consider that to be an error, so we exit the subshell with 0 in that
case.

The timeout and error escaping in this command are unportable. We can
replace it with an invocation to `litani` as follows:

    foo.out: foo.in
        litani add-job                \
          --command "my_command $^"   \
          --inputs $^                 \
          --outputs $@                \
          --timeout 90                \
          --ok-returns 10             \
          --pipeline-name my_command  \
          --ci-stage build            \
          --stdout-file $@

To actually run this, write the following shell script:

    #!/bin/sh

    litani init --project my_project
    make foo.out
    litani run-build

Running `make` doesn't actually run the job; rather, it runs `litani`,
which saves the job for later. You can run `litani add-job` as many
times as you like after running `litani init`; all these jobs are cached
and turned into a dependency graph using the arguments to `--inputs` and
`--outputs`. Running `litani run-build` runs all cached build jobs together,
in parallel if possible.

`litani` continuously updates a `run.json` file while the `run-build` command
is running, showing progress of each job as well as recording the return
codes, timeout information, and stdout/stderr of each job. This file is
documented below and is designed to be easy to render into a dashboard,
for example in HTML.


Motivation
----------

While the platform-independent job-control features are a nice bonus,
`litani`'s real value is in serving as a backend for executing a graph of
build tasks that are added from heterogeneous sources. In a complex
software project, different parts of the project can use incompatible
build systems. To build the entire project, one must either run all
build systems in parallel&mdash;potentially overcommitting on
concurrency and introducing nondeterminism when some of the targets
overlap&mdash;or run each build system serially, wasting time.

`litani` makes it possible for jobs that are specified in different build
systems to depend on each other. It also obviates the need to force
different parts of the project to use a unified build syntax, if that is
unnatural for some reason. If some parts of the build tree are specified
in Make, with others specified in CMake, then `litani` allows developers
working on each part of the codebase to use the build system that makes
sense to them, while `litani` builds the entire tree in the background.


Subtool Reference
-----------------

`litani` consists of three user-facing commands:

* `litani init`&mdash;create a new run
* `litani add-job`&mdash;add a job to the run
* `litani run-build`&mdash;run all jobs in the run in dependency order

You use `litani` by invoking all three of these commands. You firstly
initialize a run; add at least one job to the run; and then build the
run, which invokes all the jobs in dependency order.

<p>
<object
  type="image/svg+xml"
  data="litani-flow.svg"
  alt="Flow of using litani: first litani init, then one or more invocations of litani add-job, and finally litani run-build"
  id="litani-flow">
  Flow of using litani: first litani init, then one or more invocations of litani add-job, and finally litani run-build
</object>
</p>

`litani` writes all data associated with the run to a single directory;
the directory for each run is unique.  `litani`'s data format is versioned
with [semantic versioning](https://semver.org/). `litani --version`
prints the data format version and exits. The current version is 1.0.0.


### `litani init`

`litani [-v] init [-h] --project-name NAME`

Create a new run for jobs to be added to. `litani` output from subsequent
commands, as well as job artifacts and output files, will be written to
a directory associated with this run.

<p class="flag-name">
`-v, --verbose`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Verbose output. In particular, print the name of the directory to which
all output files and artifacts for this run will be written to.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--project-name NAME`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Associate this run to a "project". A project is a collection of runs.
The name of the project will be included in the JSON output for the run.
</p><!-- class="flag-desc" -->


### `litani add-job`

    litani [-v] add-job --command CMD
                        [--inputs F [F ...]]
                        [--outputs F [F ...]]
                        --pipeline-name P --ci-stage S
                        [-h] [--cwd DIR] [--timeout N] [--timeout-ok]
                        [--timeout-ignore] [--ignore-returns RC [RC ...]]
                        [--ok-returns RC [RC ...]]
                        [--interleave-stdout-stderr]
                        [--stdout-file FILE] [--stderr-file FILE]
                        [--description DESC] [--tags TAG [TAG ...]]

Describe one of the jobs to be run when `litani run-build` is invoked. A
*job* is a command that depends on inputs emits outputs. The command is
invoked when the jobs that emit each of this job's inputs have
successfully executed. Inputs and outputs can be files, but they don't
have to be; `litani` doesn't actually check whether the files were
written, just whether the job completed successfully.

#### describing the build graph:

<p class="flag-name">
`--command CMD`
</p><!-- class="flag-name" -->

<p class="flag-desc">
The command to run once all dependencies are satisfied. `CMD` is parsed
as a single string and invoked using a subshell.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--inputs F [F ...]`
<p><!-- class="flag-name" -->

<p class="flag-desc">
List of inputs that this job depends on.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--outputs F [F ...]`
</p><!-- class="flag-name" -->

<p class="flag-desc">
List of outputs that this job generates.
</p><!-- class="flag-desc" -->

#### job control:

<p class="flag-name">
`--pipeline-name P`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Which pipeline this job is a member of.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--ci-stage <build|test|report>`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Which CI stage this job is a member of.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--cwd DIR`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Directory that the command should execute in.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--timeout N`
</p><!-- class="flag-name" -->

<p class="flag-desc">
How long the job should be allowed to run for. If the timeout is
reached, the command will be terminated and the job will exit
unsuccessfully, blocking any dependant jobs from running, unless
`--timeout-ok` or `--timeout-ignore` are passed.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--timeout-ok`
</p><!-- class="flag-name" -->

<p class="flag-desc">
If the command times out, it will be terminated, but the job will be
considered successful. The pipeline that this job is part of will also
be considered successful.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--timeout-ignore`
</p><!-- class="flag-name" -->

<p class="flag-desc">
If the command times out, it will be terminated, but this will not block
dependent jobs from running.  Nevertheless, the pipeline that this job
is part of will be considered to have failed. This option is useful when
you wish to run dependant jobs even after this job has timed out, but
still want to indicate an overall failure.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--ignore-returns RC [RC ...]`
</p><!-- class="flag-name" -->

<p class="flag-desc">
If the exits with one of the specified return codes, it will not block
dependent jobs from running. Nevertheless, the pipeline that this job is part
of will be considered to have failed.  This option is useful when you
wish to run dependant jobs even after the command exited with an
abnormal status, but still want to indicate an overall failure. For
example, you may wish to generate a report as a dependant job; the
report must still be generated if this job fails.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--ok-returns RC [RC ...]`
</p><!-- class="flag-name" -->

<p class="flag-desc">
If the command exits with one of the specified return codes, the job
will still be considered successful.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--interleave-stdout-stderr`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Simulate `2>&1 >...`
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--stdout-file FILE`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Write the command's stdout to `FILE`. Note that even if this option is
not passed, `litani` will not print the command's stdout to the terminal,
but will save any output to the JSON file for the run.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--stderr-file FILE`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Write the command's stderr to `FILE`. Note that even if this option is
not passed, `litani` will not print the command's stderr to the terminal,
but will save any output to the JSON file for the run.
</p><!-- class="flag-desc" -->

#### misc:

<p class="flag-name">
`--description DESC`
</p><!-- class="flag-name" -->

<p class="flag-desc">
A human-readable string to describe the job. This will be printed to
terminal when the job is run, and can also be used in reports. `litani`
does not interpret the description, but will include it in the JSON
report for the run.
</p><!-- class="flag-desc" -->

<p class="flag-name">
`--tags TAG [TAG ...]`
</p><!-- class="flag-name" -->

<p class="flag-desc">
A list of tags for this job. `litani` does not interpret tags, but will
include them all in the JSON report for the run.
</p><!-- class="flag-desc" -->

### `litani run-build`

    litani run-build [-h] [-n] [-j N] [-o F]
                     [--pipelines P [P ...] |
                      --ci-stage <build|test|report>]

Create a new run for jobs to be added to. `litani` output from subsequent
commands, as well as job artifacts and output files, will be written to
a directory associated with this run.

<p class="flag-name">
`-j N, --parallel N`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Run at most `N` jobs in parallel. 0 means infinity, the default is based
on the number of cores on your system.
</p><!-- class="flag-desc" -->


<p class="flag-name">
`-n, --dry-run`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Don't actually run the commands in each job, just pretend that they
succeeded.
</p><!-- class="flag-desc" -->


<p class="flag-name">
`-o F, --out-file F`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Periodically write a JSON file describing the run so far to `F`. `litani`
already writes such a file to the run directory created by `litani
init`; this flag specifies an additional, more easily-accessible file to
write to.
</p><!-- class="flag-desc" -->


<p class="flag-name">
`-p P [P ...], --pipelines P [P ...]`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Only run the jobs that are associated with the pipeline `P`. This allows
you to run a subset of the jobs from start to finish.
</p><!-- class="flag-desc" -->


<p class="flag-name">
`-s <build|test|report>, --ci-stage <build|test|report>`
</p><!-- class="flag-name" -->

<p class="flag-desc">
Only run the jobs that are part of the CI stage `S`. This allows you to
run all jobs up to and including a particular stage.
</p><!-- class="flag-desc" -->


