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

HTML_DEPS = head.html index.tmp.html foot.html

index.html: $(HTML_DEPS) litani-flow.svg
	cat $(HTML_DEPS) > $@

.INTERMEDIATE: index.tmp.html
index.tmp.html: index.md
	markdown $^ > $@

litani-flow.svg: litani-flow.dot
	dot -Tsvg $^ > $@
