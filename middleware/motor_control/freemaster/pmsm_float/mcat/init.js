/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/* eslint-disable */
export class PcmObject {
    constructor() {
        this.pcm;
        this.connected = "";
        this.rpcs_addr = "localhost:41000";

    }

    onSocketConnected = () => {
        this.connected = true;
        console.log("Connected to freeMASTER");
        this.pcm.EnableEvents(true);
    }

    onSocketClose = () => {
        this.connected = false;
        console.warn("Socket closed");
    }

    onSocketError = (err) => {
        this.connected = false;
        console.error("Socket error: ", err.message);
    }

    onServerError = (err) => {
        this.connected = false;
        console.error("Server error: ", err.message);
    }

    startPcm() {
        if (typeof (this.pcm) !== "object") {
            if (typeof (FreeMASTER) !== "undefined") {
                this.rpcs_addr = FreeMASTER.rpcs_addr;
            }
            else {
                console.log("freeMaster is undefined, loading the default port...");
            }
            this.pcm = new PCM(this.rpcs_addr, this.onSocketConnected, this.onSocketClose, this.onSocketError);
            this.pcm.OnServerError = this.onServerError;
            this.pcm.OnSocketError = this.onSocketError;
        } else {
            console.log("Already connected to freeMASTER");
        }
    }

    waitUntil(attempts, callback) {
        if (attempts < 10) {
            attempts += 1;
            if (!this.connected) {
                console.log("Not connected yet, let's wait...");
                setTimeout(() => {
                    this.waitUntil(attempts, callback);
                }, 500);
            } else {
                callback();
            }
        } else {
            console.error("Unable to connect to freeMASTER after 10 times");
        }
    }

    isCommPortOpen(callback) {
        this.waitUntil(0, () => {
            this.pcm.IsCommPortOpen().then(response => callback(response)).catch(err => console.log("Error: ", err));
        })
    }

    writeOutputFileContents(filePath, contents) {
        this.waitUntil(0, () => {
            this.pcm.LocalFileOpen(filePath, "w").then((response) => {
                const fileDescriptor = response.data;
                this.pcm.LocalFileWriteString(fileDescriptor, contents, false).then((response) => {
                    console.log("Wrote " + response.data + " characters to " + filePath);
                    this.pcm.LocalFileClose(fileDescriptor).then(() => console.log("Closed: " + filePath)).catch(err => console.log("Error: " + err + " while closing: " + filePath))
                }).catch(err => {
                    console.error(err);
                });
            }).catch(err => {
                console.error(err);
            });
        });

    }

    readOutputFileContents(filePath, callback) {
        this.waitUntil(0, () => {
            this.pcm.LocalFileOpen(filePath, "r").then((response) => {
                const fileDescriptor = response.data;
                console.log("Opened " + filePath);
                this.pcm.LocalFileReadString(fileDescriptor, 0x7FFFFFFF, false).then((xd) => {
                    this.pcm.LocalFileClose(fileDescriptor).then(() => {
                        console.log("Closed: " + filePath);
                        callback(xd.data);
                    }).catch(err => {
                        console.log("Error while closing: " + filePath + ", error: " + err);
                    });
                }).catch(err => {
                    console.error(err);
                });
            }).catch(err => {
                console.log(err);
                // console.error(err);
            });
        });
    }

    checkIfFileExists(filePath) {
        return new Promise((resolve, reject) => {
            this.waitUntil(0, () => {
                this.pcm.LocalFileOpen(filePath, "r").then((response) => {
                    resolve(filePath);
                    this.pcm.LocalFileClose(response.data).then(() => {
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

    checkVariable(name) {
        return new Promise((resolve, reject) => {
            this.waitUntil(0, () => {
                this.pcm.IsCommPortOpen().then((response) => {
                    if (response.data) {
                        this.pcm.ReadVariable(name).then((data) => {
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

    writeVariable(varName, varValue) {
        this.waitUntil(0, () => {
            this.pcm.IsCommPortOpen().then((response) => {
                if (response.data) {
                    this.pcm.WriteVariable(varName, varValue).then(() => {
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

    readVariable(varName, callback) {
        this.waitUntil(0, () => {
            this.pcm.IsCommPortOpen().then((response) => {
                if (response.data) {
                    this.pcm.ReadVariable(varName).then((data) => {
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

    subscribeVariable(varName, callback) {
        this.pcm.OnVariableChanged = (name, id, value) => {
            callback(name, value);
        };
        this.waitUntil(0, () => {
            this.pcm.IsCommPortOpen().then((response) => {
                if (response.data) {
                    this.pcm.SubscribeVariable(varName).then(() => {
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

    onCommPortStateChanged(callback) {
        this.pcm.OnCommPortStateChanged = (data) => {
            console.log("Port state changed:", data);
            callback(data);
        }
    }
}
/* eslint-enable */