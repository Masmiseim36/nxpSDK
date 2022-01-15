/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/* eslint-disable */
var pcm;
var connected = "";

function waitUntil(attempts, callback) {
    if (attempts < 10) {
        attempts += 1;
        if (!connected) {
            console.log("Not connected yet, let's wait...");
            setTimeout(() => {
                waitUntil(attempts, callback);
            }, 500);
        } else {
            callback();
        }
    } else {
        console.error("Unable to connect to freeMASTER after 10 times");
    }

}

function writeOutputFileContents(filePath, contents) {
    waitUntil(0, function () {
        pcm.LocalFileOpen(filePath, "w").then((response) => {
            const fileDescriptor = response.data;
            pcm.LocalFileWriteString(fileDescriptor, contents, false).then((response) => {
                console.log("Wrote " + response.data + " characters to " + filePath);
                pcm.LocalFileClose(fileDescriptor).then(() => console.log("Closed: " + filePath)).catch(err => console.log("Error: " + err + " while closing: " + filePath))
            }).catch(err => {
                console.error(err);
            });
        }).catch(err => {
            console.error(err);
        });
    });

}

function readOutputFileContents(filePath, callback) {
    waitUntil(0, function () {
        pcm.LocalFileOpen(filePath, "r").then((response) => {
            const fileDescriptor = response.data;
            console.log("Opened " + filePath);
            pcm.LocalFileReadString(fileDescriptor, 0x7FFFFFFF, false).then((xd) => {
                pcm.LocalFileClose(fileDescriptor).then(() => {
                    console.log("Closed: " + filePath);
                    callback(xd.data);
                }).catch(err => {
                    console.log("Error while closing: " + filePath + ", error: " + err);
                });
            }).catch(err => {
                console.error(err);
            });
        }).catch(err => {
            console.error(err);
        });
    });
}

function checkIfFileExists(filePath) {
    return new Promise((resolve, reject) => {
        waitUntil(0, function () {
            pcm.LocalFileOpen(filePath, "r").then((response) => {
                resolve(filePath);
                pcm.LocalFileClose(response.data).then(() => {
                }).catch(() => {
                    console.error("Error while closing file: " + filePath);
                });
            }).catch(() => {
                // console.log("not this one", filePath);
                reject();
            });
        });
    });
}

function checkVariable(name) {
    return new Promise((resolve, reject) => {
        waitUntil(0, function () {
            pcm.IsCommPortOpen().then((response) => {
                if (response.data) {
                    pcm.ReadVariable(name).then((data) => {
                        resolve(data.data);
                    }).catch((error) => {
                        reject(error);
                    });
                } else {
                    reject("Cannot read variable - communication port is not open");
                }
            });
        });
    });


}

function writeVariable(varName, varValue) {
    waitUntil(0, function () {
        pcm.IsCommPortOpen().then((response) => {
            if (response.data) {
                pcm.WriteVariable(varName, varValue).then(() => {
                    console.log("Variable", varName, "was updated with value:", varValue);
                }).catch((error) => {
                    console.log(error.msg);
                });
            } else {
                console.warn("Cannot write variable to freeMaster - communication port is not open.");
            }
        });
    });
}

function readVariable(varName, callback) {
    waitUntil(0, function () {
        pcm.IsCommPortOpen().then((response) => {
            if (response.data) {
                pcm.ReadVariable(varName).then((data) => {
                    // console.log("Variable: ", varName, " was found in freeMaster and its value is: ", data.xtra.formatted);
                    callback(data.xtra);
                }).catch((error) => {
                    console.log("Cannot find value: ", varName, ", error message is: ", error.msg);
                    callback("");
                });
            } else {
                console.warn("Cannot read variable - communication port is not open.");
                callback("");
            }
        });
    });
}

function subscribeVariable(varName, callback) {
    pcm.OnVariableChanged = (name, id, value) => {
        callback(name, value);
    };
    waitUntil(0, function () {
        pcm.IsCommPortOpen().then((response) => {
            if (response.data) {
                pcm.SubscribeVariable(varName, 2000).then(() => {
                    // console.log("Variable ", varName + " was successfully subscribed");
                }).catch((error) => {
                    console.log("Error: ", error);
                })
            } else {
                console.warn("Cannot subscribe variable - communication port is not open");
            }
        })
    })
}

function onSocketConnected() {
    connected = true;
    console.log("Connected to freeMASTER");
    pcm.EnableExtraFeatures(true);
    pcm.EnableEvents(true);
}

function onSocketClose() {
    connected = false;
    console.warn("Socket closed");
}

function onSocketError(err) {
    connected = false;
    console.error("Socket error: ", err.message);
}

function onServerError(err) {
    connected = false;
    console.error("Server error: ", err.message);
}

function startPcm() {
    if (typeof (pcm) !== "object") {
        pcm = new PCM("localhost:41000", onSocketConnected, onSocketClose, onSocketError);
        pcm.OnServerError = onServerError;
        pcm.OnSocketError = onSocketError;
    } else {
        console.log("Already connected to freeMASTER");
    }
}

export default [startPcm, writeVariable, checkVariable, writeOutputFileContents, readOutputFileContents, checkIfFileExists, readVariable, subscribeVariable];
/* eslint-enable */