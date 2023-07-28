/*
 * Copyright 2018-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Project: FreeMASTER JSON-RPC Client
 * freemaster-client.js: FreeMASTER client library
 */

/**
 * @typedef {Object} CommPortInfo
 *
 * Communication port information.
 *
 * @property {string} name              Communication port friendly name
 * @property {string} description       Communication port description
 * @property {string} connection_string Connection string
 * @property {string} elf               Elf file path
 */

/**
 * @typedef {Object} BoardInfo
 *
 * Detected board information.
 *
 * @property {number} protVer      Protocol version
 * @property {number} cfgFlags     Configuration flags
 * @property {number} dataBusWdt   Data bus width
 * @property {number} globVerMajor Major version
 * @property {number} globVerMinor Minor version
 * @property {number} cmdBuffSize  Command buffer size
 * @property {number} recBuffSize  Receive buffer size
 * @property {number} recTimeBase  Recorder time base
 * @property {string} descr        Description
 */

/**
 * @typedef {Object} SymbolInfo
 *
 * Symbol information.
 *
 * @property {string} name Symbol name
 * @property {number} addr Symbol address
 * @property {number} size Symbol size
 * @property {string} type Symbol type
 */

/**
 * @typedef {Object} VariableInfo
 *
 * Variable information.
 *
 * @property {string} name    Variable name
 * @property {number} addr    Variable address
 * @property {string} type    Variable type (int, uint, fract, ufract, float, or double)
 * @property {number} size    Variable size (1, 2, 4, or 8)
 * @property {number} [shift] Number of shift positions (integer variable)
 * @property {number} [mask]  And mask applied on integer variable
 * @property {number} q_n     Number of bits designating fractional portion of fractional variable
 * @property {number} q_m     Number of bits designating integer portion of fractional variable
 */

/**
 * @typedef {Object} RecorderLimits
 *
 * Recorder limits.
 *
 * @property {number} baseRate_ns   Base time at which recorder operates in nanoseconds (0 when unknown or not deterministic)
 * @property {number} buffSize      Total recorder memory size
 * @property {number} recStructSize Overhead structure size (protocol version > 4.0)
 * @property {number} varStructSize Per-variable overhead structure size (protocol version > 4.0)
 */

/**
 * @typedef {Object} RecorderConfig
 *
 * Recorder configuration.
 *
 * @property {number} pointsTotal      Total number of recorded points per variable
 * @property {number} pointsPreTrigger Number of recorded points before trigger
 * @property {number} timeDiv          Time-base multiplier
 */

/**
 * @typedef {Object} RecorderVariable
 *
 * Recorder variable information.
 *
 * @property {string} name    Variable name
 * @property {number} trgType Trigger type (if missing variable is not used as trigger)
 *
 * | Mask | Description                            |
 * | ---- | -------------------------------------- |
 * | 0x01 | reserved                               |
 * | 0x02 | reserved                               |
 * | 0x04 | trigger-only                           |
 * | 0x08 | reserved                               |
 * | 0x10 | trigger on rising edge _/              |
 * | 0x20 | trigger on falling edge \_             |
 * | 0x40 | 0=normal edge trigger, 1=level trigger |
 * | 0x80 | use variable threshold                 |
 * @property {number} trgThr Trigger threshold (if missing variable is not used as trigger)
 */

(function(root) {
  'use strict';

  if (typeof require !== 'undefined') {
    root.simple_jsonrpc = require('simple-jsonrpc-js');
    root.WebSocket = require('ws');
  }

  /**
   * @constructs PCM
   * @classdesc PCM is an adapter class for FreeMASTER Lite API that handles the websocket connections to the service and command conversion to JSON-RPC format. It can be used both on front-end (web browsers) and back-end (NodeJS).
   * @description Creates an instance of PCM object.
   *
   * @example
   * // Create a PCM instance
   * function main() {
   *     // main logic of the application
   * };
   * $(document).ready(function() {
   *     var pcm = new PCM(window.location.host, main);
   * });
   *
   * @example
   * // Handle API calls using {@link https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise Promises}
   * pcm.PCM_Function(params)
   *     .then(response => {
   *         console.log("Procedure call succeeded and returned ", response.data);
   *     })
   *     .catch(error => {
   *         console.log("Procedure call failed with the following error ", error);
   *     });
   *
   * @example
   * // Handle API calls as {@link https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/await async functions}
   * try {
   *     let response = await pcm.PCM_Function(params);
   *     console.log("Procedure call succeeded and returned ", response.data);
   * } catch (err) {
   *     console.log("Procedure call failed with the following error ", error);
   * }
   *
   * @param {string}   url           The address of the web server.
   * @param {Function} onSocketOpen  WebSocket open event handler.
   * @param {Function} onSocketClose WebSocket close event handler.
   * @param {Function} onSocketError WebSocket error event handler.
   */
  var PCM = function(url, onSocketOpen, onSocketClose, onSocketError) {

    if (!url.startsWith('ws://') && !url.startsWith('wss://')) {
      url = 'ws://' + url;
    }
    this.OnSocketOpen = onSocketOpen || console.log;
    this.OnSocketClose = onSocketClose || console.log;
    this.OnSocketError = onSocketError || console.log;
    this.OnServerError = console.log;

    this._socket = new root.WebSocket(url);
    this._jrpc = new root.simple_jsonrpc();

    this._jrpc.toStream = function(_msg) {
      this._socket.send(_msg);
    }.bind(this);

    this._socket.onopen = function(event) {
      this.OnSocketOpen(event);
    }.bind(this);

    this._socket.onclose = function(event) {
      this.OnSocketClose(event);
    }.bind(this);

    this._socket.onerror = function(event) {
      this.OnSocketError(event);
    }.bind(this);

    this._socket.onmessage = function(event) {
      this._jrpc.messageHandler(event.data);
    }.bind(this);

    this.SendRequest = function(method, args) {
      return new Promise((resolve, reject) => {
        this._jrpc.call(method, args)
          .then((response) => {
            if (response.success)
              resolve(response);
            else
              reject(response.error);
          })
          .catch((error) => {
            reject(error);
            this.OnServerError(error);
          });
      });
    };

    /* Install default FreeMASTER event handlers. Don't forget to call EnableEvents(true) for the server to generate the JSON-RPC events. */
    this.OnBoardDetected = function() {
      console.log('FreeMASTER Event received: OnBoardDetected()');
    };

    this.OnCommPortStateChanged = function(state) {
      console.log('FreeMASTER Event received: OnCommPortStateChanged(' + state + ')');
    };

    this.OnVariableChanged = function(name, id, value) {
      console.log('FreeMASTER Event received: OnVariableChanged("' + name + '", ' + id + ', ' + value + ')');
    };

    this.OnRecorderDone = function() {
      console.log('FreeMASTER Event received: OnRecorderDone()');
    };

    this.OnCustomEvent = function(arg1) {
      console.log('FreeMASTER Event received: OnCustomEvent(' + JSON.stringify(arg1) + ')');
    };

    /* Register event handlers in JSON-RPC */
    this._jrpc.dispatch('OnBoardDetected', 'pass', (params_array) => this.DispatchEvent(this.OnBoardDetected, params_array));
    this._jrpc.dispatch('OnCommPortStateChanged', 'pass', (params_array) => this.DispatchEvent(this.OnCommPortStateChanged, params_array));
    this._jrpc.dispatch('OnVariableChanged', 'pass', (params_array) => this.DispatchEvent(this.OnVariableChanged, params_array));
    this._jrpc.dispatch('OnRecorderDone', 'pass', (params_array) => this.DispatchEvent(this.OnRecorderDone, params_array));
    this._jrpc.dispatch('OnCustomEvent', 'pass', (params_array) => this.DispatchEvent(this.OnCustomEvent, params_array));
  };

  /**
   * Closes WebSocket instantly disconnecting PCM instance from the server.
   *
   * @example
   * pcm.Shutdown();
   */
  PCM.prototype.Shutdown = function() {
    this._socket.close();
  };

  /**
   * Dispatches a JSON-RPC notification event
   */
   PCM.prototype.DispatchEvent = function(func, params_array) {
    try {
      func.apply(null, params_array);
    }
    catch(e) {
      // error in dispatched callback (possibly a syntax error in user code)
      var errMsg = (e instanceof Error) ? (e.name + ": " + e.message) : JSON.stringify(e);
      console.error("Error in " + func.name + " event handler: " + errMsg);
    }
  }

  /**
   * Requests FreeMASTER/FreeMASTER Lite version.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.GetAppVersion().then(response => console.log("App version ", response.data));
   *
   * @returns {Promise} In case of success, resolved promise will contain data property of type string representing the version.
   */
  PCM.prototype.GetAppVersion = function() {
    return this.SendRequest.call(this, 'GetAppVersion');
  };

  /**
   * Requests communication port name (defined in project file) by index.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#GetCommPortInfo GetCommPortInfo}
   *
   * @example
   * let index = 0;
   * do {
   *     try {
   *         let response = await pcm.EnumCommPorts(index);
   *         console.log(response.data);
   *         index = index + 1;
   *     } catch (err) {
   *         break;
   *     }
   * } while (true);
   *
   * @param   {number} index Communication port index.
   * @returns {Promise} In case of success, resolved promise will contain data property of type string representing the connection friendly name.
   */
  PCM.prototype.EnumCommPorts = function(index) {
    return this.SendRequest.call(this, 'EnumCommPorts', [index]);
  };

  /**
   * Requests communication port information (defined in project file).
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#EnumCommPorts EnumCommPorts}
   *
   * @example
   * pcm.EnumCommPorts(0).then(response => {
   *     pcm.GetCommPortInfo(response.data).then(response => {
   *         console.log(response.data);
   *     });
   * });
   *
   * @param   {string} name Communication port friendly name returned by {@link PCM#EnumCommPorts EnumCommPorts}
   * @returns {Promise} In case of success, resolved promise will contain data property of type {@link CommPortInfo CommPortInfo}.
   */
  PCM.prototype.GetCommPortInfo = function(name) {
    return this.SendRequest.call(this, 'GetCommPortInfo', [name]);
  };

  /**
   * Starts communication using connection friendly name  (defined in project file) or connection string.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.StartComm("PortX").then(() => console.log("Communication port open."));
   *
   * @param   {string} name Connection friendly name or connection string.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.StartComm = function(name) {
    return this.SendRequest.call(this, 'StartComm', [name]);
  };

  /**
   * Stops communication.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.StopComm().then(() => console.log("Communication port closed."));
   *
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.StopComm = function() {
    return this.SendRequest.call(this, 'StopComm');
  };

  /**
   * Checks if communication port is open.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.IsCommPortOpen().then(response => console.log("Is port open ? ", response.data));
   *
   * @returns {Promise} In case of success, resolved promise will contain data property of type bool.
   */
  PCM.prototype.IsCommPortOpen = function() {
    return this.SendRequest.call(this, 'IsCommPortOpen');
  };

  /**
   * Checks if the board was detected.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.IsBoardDetected().then(response => console.log("Is board detected ? ", response.data));
   *
   * @returns {Promise} In case of success, resolved promise will contain data property of type bool.
   */
  PCM.prototype.IsBoardDetected = function() {
    return this.SendRequest.call(this, 'IsBoardDetected');
  };

  /**
   * Requests detected board information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.GetDetectedBoardInfo().then(response => console.log("Board information: ", response.data));
   *
   * @deprecated Since Protocol Version 4.0
   *
   * @returns {Promise} In case of success, resolved promise will contain data property of type {@link BoardInfo BoardInfo} representing board information.
   */
  PCM.prototype.GetDetectedBoardInfo = function() {
    return this.SendRequest.call(this, 'GetDetectedBoardInfo');
  };

  /**
   * Requests configuration parameter of type uint8.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.GetConfigParamU8("F1").then(response => console.log("F1: ", response.data));
   *
   * @since Protocol Version 4.0
   *
   * @param   {string} name Parameter name
   *
   * | Name | Description                                   |
   * | :--- | :---------------------------------------------|
   * | F1   | Flags                                         |
   * | RC   | Number of recorders implemented on target     |
   * | SC   | Number of oscilloscopes implemented on target |
   * | PC   | Number of pipes implemented on target         |
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing parameter value.
   */
  PCM.prototype.GetConfigParamU8 = function(name) {
    return this.SendRequest.call(this, 'GetConfigParamU8', [name]);
  };

  /**
   * Requests configuration parameter encoded as ULEB128.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.GetConfigParamULEB("MTU").then(response => console.log("MTU: ", response.data));
   *
   * @since Protocol Version 4.0
   *
   * @param   {string} name Parameter name
   *
   * | Name | Description                                                                       |
   * | :--- | :-------------------------------------------------------------------------------- |
   * | MTU  | Size of an internal communication buffer for handling command and response frames |
   * | BA   | Base address used by optimized memory read/write commands                         |
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing parameter value.
   */
  PCM.prototype.GetConfigParamULEB = function(name) {
    return this.SendRequest.call(this, 'GetConfigParamULEB', [name]);
  };

  /**
   * Requests configuration parameter of type string.
   *
   * | Name | Description             |
   * | :--- | :-----------------------|
   * | VS   | Version string          |
   * | NM   | Application name string |
   * | DS   | Description string      |
   * | BD   | Build date/time string  |
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.GetConfigParamString("VS", 10).then(response => console.log("VS: ", response.data));
   *
   * @since Protocol Version 4.0
   *
   * @param   {string} name  Parameter name
   * @param   {number} [len] String byte length, if missing will be set to the service max buffer size (256)
   * @returns {Promise} In case of success, resolved promise will contain data property of type string representing parameter value.
   */
  PCM.prototype.GetConfigParamString = function(name, len) {
    return this.SendRequest.call(this, 'GetConfigParamString', [name, len]);
  };

  /**
   * Requests specific access level to the target board functionalities using a password.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @example
   * pcm.Authenticate("my_pass", 2).then(response => console.log(response.data));
   *
   * @param   {string} pass   Password corresponding to requested access level.
   * @param   {number} access Access level 1 - read, 2 - read & write, 3 - read, write & flash.
   * @returns {Promise} In case of success, resolved promise will contain granted access level.
   */
  PCM.prototype.Authenticate = function(pass, access) {
    return this.SendRequest.call(this, 'Authenticate', [pass, access]);
  };

  /**
   * Reads a signed integer value from a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.ReadIntVariable(0x20050080, 2).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        size   Integer size, can be 1, 2, 4, or 8.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the read value.
   */
  PCM.prototype.ReadIntVariable = function(addr, size) {
    return this.SendRequest.call(this, 'ReadIntVariable', [addr, size]);
  };

  /**
   * Reads an unsigned integer value from a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.ReadUIntVariable('var16', 4).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        size   Integer size, can be 1, 2, 4, or 8.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the read value.
   */
  PCM.prototype.ReadUIntVariable = function(addr, size) {
    return this.SendRequest.call(this, 'ReadUIntVariable', [addr, size]);
  };

  /**
   * Reads a floating-point value from a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.ReadFloatVariable('varFLT').then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the read value.
   */
  PCM.prototype.ReadFloatVariable = function(addr) {
    return this.SendRequest.call(this, 'ReadFloatVariable', [addr]);
  };

  /**
   * Reads a double value from a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.ReadDoubleVariable('varDBL').then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the read value.
   */
  PCM.prototype.ReadDoubleVariable = function(addr) {
    return this.SendRequest.call(this, 'ReadDoubleVariable', [addr]);
  };

  /**
   * Writes a signed integer value to a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.WriteIntVariable(0x20050080, 2, 10).then(() => console.log('Value written.'));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        size   Integer size, can be 1, 2, 4, or 8.
   * @param   {number}        data   Integer value to be written.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.WriteIntVariable = function(addr, size, data) {
    return this.SendRequest.call(this, 'WriteIntVariable', [addr, size, data]);
  };

  /**
   * Writes an unsigned integer value to a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.WriteUIntVariable("var16", 2, 100).then(() => console.log('Value written.'));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        size   Integer size, can be 1, 2, 4, or 8.
   * @param   {number}        data   Integer value to be written.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.WriteUIntVariable = function(addr, size, data) {
    return this.SendRequest.call(this, 'WriteUIntVariable', [addr, size, data]);
  };

  /**
   * Writes a floating-point value to a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.WriteFloatVariable("varFLT", 10.0).then(() => console.log('Value written.'));
   *
   * @param   {number|string} addr Address value or symbol name.
   * @param   {number}        data Floating-point value to be written.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.WriteFloatVariable = function(addr, data) {
    return this.SendRequest.call(this, 'WriteFloatVariable', [addr, data]);
  };

  /**
   * Writes a double value to a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.WriteDoubleVariable("varDBL", 100.0).then(() => console.log('Value written.'));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        data   Double value to be written.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.WriteDoubleVariable = function(addr, data) {
    return this.SendRequest.call(this, 'WriteDoubleVariable', [addr, data]);
  };

  /**
   * Returns an array of bytes of the requested size from a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * // read 20 bytes from address 0x20050080
   * pcm.ReadMemory(0x20050080, 20).then(response => console.log(response.data));
   *
   * @param   {number} addr   Address value.
   * @param   {number} size   Number of elements.
   * @returns {Promise} In case of success, resolved promise will contain data property of type Array<number>.
   */
  PCM.prototype.ReadMemory = function(addr, size) {
    return this.SendRequest.call(this, 'ReadMemory', [addr, size]);
  };

  /**
   * Reads an array of signed integers from a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * // read 20, 2 byte long, signed integers from address 0x20050080
   * pcm.ReadIntArray(0x20050080, 20, 2).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        size   Number of elements.
   * @param   {number}        elSize Element size, can be 1, 2, 4, or 8.
   * @returns {Promise} In case of success, resolved promise will contain data property of type Array<number>.
   */
  PCM.prototype.ReadIntArray = function(addr, size, elSize) {
    return this.SendRequest.call(this, 'ReadIntArray', [addr, size, elSize]);
  };

  /**
   * Reads an array of unsigned integers from a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * // read 10, 4 byte long, unsigned integers from the address given by the symbol 'arr16'
   * pcm.ReadUIntArray('arr16', 10, 4).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        size   Number of elements.
   * @param   {number}        elSize Element size, can be 1, 2, 4, or 8.
   * @returns {Promise} In case of success, resolved promise will contain data property of type Array<number>.
   */
  PCM.prototype.ReadUIntArray = function(addr, size, elSize) {
    return this.SendRequest.call(this, 'ReadUIntArray', [addr, size, elSize]);
  };

  /**
   * Reads an array of floating-point numbers from a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * // read 5 floats from the address given by the symbol 'arrFLT'
   * pcm.ReadFloatArray('arrFLT', 5).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        size   Number of elements.
   * @returns {Promise} In case of success, resolved promise will contain data property of type Array<number>.
   */
  PCM.prototype.ReadFloatArray = function(addr, size) {
    return this.SendRequest.call(this, 'ReadFloatArray', [addr, size]);
  };

  /**
   * Reads an array of double-precision numbers from a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * // read 5 double-precision numbers from the address given by the symbol 'arrDBL'
   * pcm.ReadDoubleArray('arrDBL', 5).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        size   Number of elements.
   * @returns {Promise} In case of success, resolved promise will contain data property of type Array<number>.
   */
  PCM.prototype.ReadDoubleArray = function(addr, size) {
    return this.SendRequest.call(this, 'ReadDoubleArray', [addr, size]);
  };

  /**
   * Writes an array of bytes to a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.WriteMemory(0x20050080, [1, 2, 3, 4, 5]).then(response => console.log(response.data));
   *
   * @param   {number}        addr   Address value.
   * @param   {Array<number>} data   Array of bytes to be written.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written array elements.
   */
  PCM.prototype.WriteMemory = function(addr, data) {
    return this.SendRequest.call(this, 'WriteMemory', [addr, data]);
  };

  /**
   * Writes an array of signed integers to a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.WriteIntArray(0x20050080, 2, [1, 2, 3, 4, 5]).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        elSize Element size, can be 1, 2, 4, or 8.
   * @param   {Array<number>} data   Array of integers to be written.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written array elements.
   */
  PCM.prototype.WriteIntArray = function(addr, elSize, data) {
    return this.SendRequest.call(this, 'WriteIntArray', [addr, elSize, data]);
  };

  /**
   * Writes an array of unsigned integers to a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.WriteUIntArray('arr16', 4, [100, 1000, 10000, 100000, 1000000]).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {number}        elSize Element size, can be 1, 2, 4, or 8.
   * @param   {Array<number>} data   Array of integers to be written.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written array elements.
   */
  PCM.prototype.WriteUIntArray = function(addr, elSize, data) {
    return this.SendRequest.call(this, 'WriteUIntArray', [addr, elSize, data]);
  };

  /**
   * Writes an array of floating-point numbers to a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.ReadFloatArray('arrFLT', [1.0, 2.0, 3.0, 4.0, 5.0]).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {Array<number>} data   Array of floating-point numbers to be written.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written array elements.
   */
  PCM.prototype.WriteFloatArray = function(addr, data) {
    return this.SendRequest.call(this, 'WriteFloatArray', [addr, data]);
  };

  /**
   * Writes an array of double-precision numbers to a memory location.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.WriteDoubleArray('arrDBL', [1.0, 2.0, 3.0, 4.0, 5.0]).then(response => console.log(response.data));
   *
   * @param   {number|string} addr   Address value or symbol name.
   * @param   {Array<number>} data   Array of double-precision numbers to be written.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written array elements.
   */
  PCM.prototype.WriteDoubleArray = function(addr, data) {
    return this.SendRequest.call(this, 'WriteDoubleArray', [addr, data]);
  };

  /**
   * Sends the command to read symbols from the elf file associated with the current connection (defined in project file).
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#EnumSymbols EnumSymbols}
   *
   * @param {string} [elfFile] Optional path to the elf file.
   *
   * @example
   * pcm.ReadELF().then(response => console.log(response.data.count + " symbols extracted from ELF file."));
   *
   * @returns {Promise} In case of success, resolved promise will contain the number of extracted symbols.
   */
  PCM.prototype.ReadELF = function(elfFile) {
    return this.SendRequest.call(this, 'ReadELF', [elfFile]);
  };

  /**
   * Sends the command to read symbols from the TSA table from the connected target.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#EnumSymbols EnumSymbols}
   *
   * @example
   * pcm.ReadTSA().then(response => console.log(response.data.count + " symbols extracted from TSA table."));
   *
   * @returns {Promise} In case of success, resolved promise will contain the number of extracted symbols.
   */
  PCM.prototype.ReadTSA = function() {
    return this.SendRequest.call(this, 'ReadTSA');
  };

  /**
   * Requests symbol (extracted from ELF file or TSA table) name by index.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#ReadELF ReadELF}
   * @see {@link PCM#ReadTSA ReadTSA}
   * @see {@link PCM#GetSymbolInfo GetSymbolInfo}
   *
   * @example
   * let index = 0;
   * do {
   *     try {
   *         let response = await pcm.EnumSymbols(index);
   *         console.log(response.data);
   *         index = index + 1;
   *     } catch (err) {
   *         break;
   *     }
   * } while (true);
   *
   * @param   {number} index Symbol index.
   * @returns {Promise} In case of success, resolved promise will contain data property of type string representing the symbol name.
   */
  PCM.prototype.EnumSymbols = function(index) {
    return this.SendRequest.call(this, 'EnumSymbols', [index]);
  };

  /**
   * Requests symbol information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#EnumSymbols EnumSymbols}
   *
   * @example
   * pcm.EnumSymbols(0).then(response => {
   *     pcm.GetSymbolInfo(response.data).then(response => {
   *         console.log(response.data);
   *     });
   * });
   *
   * @param   {string} name Symbol name returned by {@link PCM#EnumSymbols EnumSymbols}
   * @returns {Promise} In case of success, resolved promise will contain data property of type {@link SymbolInfo SymbolInfo}.
   */
  PCM.prototype.GetSymbolInfo = function(name) {
    return this.SendRequest.call(this, 'GetSymbolInfo', [name]);
  };

  /**
   * Requests variable name by index.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#DefineVariable DefineVariable}
   * @see {@link PCM#GetVariableInfo GetVariableInfo}
   *
   * @example
   * let index = 0;
   * do {
   *     try {
   *     let response = await pcm.EnumVariables(index);
   *     console.log(response.data);
   *     index = index + 1;
   *     } catch (err) {
   *         break;
   *     }
   * } while (true);
   *
   * @param   {number} index Variable index.
   * @returns {Promise} In case of success, resolved promise will contain data property of type string representing the variable name.
   */
  PCM.prototype.EnumVariables = function(index) {
    return this.SendRequest.call(this, 'EnumVariables', [index]);
  };

  /**
   * Requests variable information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#DefineVariable DefineVariable}
   *
   * @example
   * pcm.EnumVariables(0).then(response => {
   *     pcm.GetVariableInfo(response.data).then(response => {
   *         console.log(response.data);
   *     });
   * });
   *
   * @param   {string} name Variable name.
   * @returns {Promise} In case of success, resolved promise will contain data property of type {@link VariableInfo VariableInfo}.
   */
  PCM.prototype.GetVariableInfo = function(name) {
    return this.SendRequest.call(this, 'GetVariableInfo', [name]);
  };

  /**
   * Defines a variable.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * let variable = { name: "var16", addr: 0x20050080, type: "uint", size: 4 }
   * pcm.DefineVariable(variable).then(() => console.log("Variable successfully defined."));
   *
   * @param   {VariableInfo} variable Variable information
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DefineVariable = function(variable) {
    return this.SendRequest.call(this, 'DefineVariable', [variable]);
  };

  /**
   * Delete an user defined variable (except those defined in project file).
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @example
   * pcm.DeleteVariable("var16").then(() => console.log("Variable deleted."));
   *
   * @param   {string} name Variable name.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DeleteVariable = function(name) {
    return this.SendRequest.call(this, 'DeleteVariable', [name]);
  };

  /**
   * Deletes all user defined variables (except those defined in project file).
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @example
   * pcm.DeleteAllScriptVariables().then(() => console.log("Script variables deleted."));
   *
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DeleteAllScriptVariables = function() {
    return this.SendRequest.call(this, 'DeleteAllScriptVariables');
  };

  /**
   * Reads variable value according to the predefined variable information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#DefineVariable DefineVariable}
   *
   * @example
   * pcm.ReadVariable("var16").then(response => console.log(response.data));
   *
   * @param   {string} name Variable name.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing variable value.
   */
  PCM.prototype.ReadVariable = function(name) {
    return this.SendRequest.call(this, 'ReadVariable', [name]);
  };

  /**
   * Writes a variable value according to the predefined variable information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#DefineVariable DefineVariable}
   *
   * @example
   * pcm.WriteVariable("var16", 255).then(() => console.log("Value successfully written."));
   *
   * @param   {string} name  Variable name.
   * @param   {number} value Value to be written.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.WriteVariable = function(name, value) {
    return this.SendRequest.call(this, 'WriteVariable', [name, value]);
  };

  /**
   * Reads multiple variables values according to the predefined variable information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#DefineVariable DefineVariable}
   *
   * @example
   * pcm.ReadMultipleVariables(["var8", "var16", "var32"]).then(response => console.log(response.data));
   *
   * @param   {Array<string>} names Variables names.
   * @returns {Promise} In case of success, resolved promise will contain data property of type array.
   * Each array element will include variable `name` and operation `status` and `value` in case status is `true`.
   */
  PCM.prototype.ReadMultipleVariables = function(names) {
    return this.SendRequest.call(this, 'ReadMultipleVariables', [names]);
  };

  /**
   * Writes multiple variables values according to the predefined variable information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#DefineVariable DefineVariable}
   *
   * @example
   * pcm.WriteMultipleVariables([{ name: "var8", value: 255 }, { name: "varFLT", value: 3.14 }]).then((data) => console.log(data));
   *
   * @param   {Array<object>} values Array of objects containing variable `name` and `value`.
   * @returns {Promise} In case of success, resolved promise will contain data property of type array.
   * Each array element will include variable `name` and operation `status`.
   */
  PCM.prototype.WriteMultipleVariables = function(values) {
    return this.SendRequest.call(this, 'WriteMultipleVariables', [values]);
  };

  /**
   * Setups an oscilloscope with a specific ID.
   *
   * Notes:
   * * Scope ID should be in the target supported range (defined in the embedded application).
   * * All the variables should be defined prior to scope definition.
   * * Older protocol version (< 4.0) support only one scope instance.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#GetOscilloscopeData GetOscilloscopeData}
   *
   * @example
   * let id = 0;
   * let vars = ['myVAr1', 'myVar2', 'myVar3'];
   * pcm.SetupOscilloscope(id, vars).then(() => console.log("Scope was setup successfully."));
   *
   * @param   {number}        id   Oscilloscope ID.
   * @param   {Array<string>} vars Oscilloscope variables names.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.SetupOscilloscope = function(id, vars) {
    return this.SendRequest.call(this, 'SetupOscilloscope', [id, vars]);
  };

  /**
   * Requests oscilloscope data.
   *
   * The values will be returned in the format defined by each variable.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#SetupOscilloscope SetupOscilloscope}
   *
   * @example
   * let id = 0;
   * let vars = ['myVAr1', 'myVar2', 'myVar3'];
   * pcm.SetupOscilloscope(id, vars).then(() => {
   *     pcm.GetOscilloscopeData(id).then(response => console.log(response.data));
   * });
   *
   * @param   {number} id Scope ID.
   * @returns {Promise} In case of success, resolved promise will contain data property of type array of numbers (variables corresponding values in the defined order).
   */
  PCM.prototype.GetOscilloscopeData = function(id) {
    return this.SendRequest.call(this, 'GetOscilloscopeData', [id]);
  };

  /**
   * Requests recorder limits.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @example
   * let id = 0;
   * pcm.GetRecorderLimits(id).then(response => console.log(response.data));
   *
   * @param   {number} id Recorder ID.
   * @returns {Promise} In case of success, resolved promise will contain data property of type {@link RecorderLimits RecorderLimits}.
   */
  PCM.prototype.GetRecorderLimits = function(id) {
    return this.SendRequest.call(this, 'GetRecorderLimits', [id]);
  };

  /**
   * Setups a recorder with a specific ID.
   *
   * Notes:
   * * Recorder ID should be in the target supported range (defined in the embedded application).
   * * All the variables should be defined prior to recorder definitions.
   * * Older protocol version (< 4.0) support only one recorder instance.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @example
   * let id = 0;
   * let config = {
   *     pointsTotal: 100,
   *     pointsPreTrigger: 50,
   *     timeDiv: 1
   * };
   * // myVar1 & myVar3 will be recorded (only 'name' property is required)
   * // myVar2 will be used as trigger on rising edge (0x04 | 0x10) with 2000 threshold value
   * let vars = [{ name: 'myVar1'}, { name: 'myVar2', trgType: 0x14, trgThr: 2000 }, { name: 'myVar3'}];
   * pcm.SetupRecorder(id, config, vars).then(() => console.log("Recorder was setup successfully."));
   *
   * @param   {number}                 id      Recorder ID.
   * @param   {RecorderConfig}         config  Recorder configuration.
   * @param   {Array<RecorderVariable>} vars    Trigger variables.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.SetupRecorder = function(id, config, vars) {
    return this.SendRequest.call(this, 'SetupRecorder', [id, config, vars]);
  };

  /**
   * Starts a recorder.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#SetupRecorder SetupRecorder}
   *
   * @example
   * pcm.StartRecorder(id).then(() => console.log("Recorder started"));
   *
   * @param   {number} id Recorder ID.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.StartRecorder = function(id) {
    return this.SendRequest.call(this, 'StartRecorder', [id]);
  };

  /**
   * Stops a recorder.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#SetupRecorder SetupRecorder}
   * @see {@link PCM#StartRecorder StartRecorder}
   *
   * @example
   * pcm.StopRecorder(id).then(() => console.log("Recorder stopped"));
   *
   * @param   {number} id Recorder ID.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.StopRecorder = function(id) {
    return this.SendRequest.call(this, 'StopRecorder', [id]);
  };

  /**
   * Requests recorder status.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#SetupRecorder SetupRecorder}
   * @see {@link PCM#StartRecorder StartRecorder}
   *
   * @example
   * pcm.GetRecorderStatus(id).then(response => console.log(response.data));
   *
   * @param   {number} id Recorder ID.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number.
   *
   * | Code | Status                           |
   * | :--- | :------------------------------- |
   * | 0x00 | not configured                   |
   * | 0x01 | configured, stopped, no data     |
   * | 0x02 | running                          |
   * | 0x03 | stopped, not enough data sampled |
   * | 0x04 | stopped, data ready              |
   */
  PCM.prototype.GetRecorderStatus = function(id) {
    return this.SendRequest.call(this, 'GetRecorderStatus', [id]);
  };

  /**
   * Gets recorded data.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @example
   * pcm.GetRecorderData(id).then(response => console.log(response.data));
   *
   * @param   {number} id Recorder ID.
   * @returns {Promise} In case of success, resolved promise will contain data property of type array of arrays of numbers.
   */
  PCM.prototype.GetRecorderData = function(id) {
    return this.SendRequest.call(this, 'GetRecorderData', [id]);
  };

  /**
   * Opens a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeOpen(0, 100, 100).then(() => console.log("Pipe open."));
   *
   * @param   {number} port         Pipe ID.
   * @param   {number} txBufferSize Send buffer size.
   * @param   {number} rxBufferSize Receive buffer size.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.PipeOpen = function(port, txBufferSize, rxBufferSize) {
    return this.SendRequest.call(this, 'PipeOpen', [port, txBufferSize, rxBufferSize]);
  };

  /**
   * Closes a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeClose(0).then(() => console.log("Pipe closed."));
   *
   * @param   {number} port Pipe ID.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.PipeClose = function(port) {
    return this.SendRequest.call(this, 'PipeClose', [port]);
  };

  /**
   * Flushes a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeFlush(0).then(() => console.log("Pipe flushed."));
   *
   * @param   {number} port Pipe ID.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.PipeFlush = function(port, timeout) {
    return this.SendRequest.call(this, 'PipeFlush', [port, timeout]);
  };

  /**
   * Sets Pipe default receive mode.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeSetDefaultRxMode(false, 100).then(() => console.log("Default RX mode updated."));
   *
   * @param   {boolean} rxAllOrNothing Flag specifying whether the data should be read all at once.
   * @param   {number}  rxTimeout_ms   Read timeout in milliseconds.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.PipeSetDefaultRxMode = function(rxAllOrNothing, rxTimeout_ms) {
    return this.SendRequest.call(this, 'PipeSetDefaultRxMode', [rxAllOrNothing, rxTimeout_ms]);
  };

  /**
   * Sets Pipe default transmit mode.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   *
   * @example
   * pcm.PipeSetDefaultTxMode(false).then(() => console.log("Default TX mode updated."));
   *
   * @param   {boolean} txAllOrNothing Flag specifying whether the data should be sent all at once.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.PipeSetDefaultTxMode = function(txAllOrNothing) {
    return this.SendRequest.call(this, 'PipeSetDefaultTxMode', [txAllOrNothing]);
  };

  /**
   * Sets Pipe default string mode.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeSetDefaultStringMode(false).then(() => console.log("Default string mode updated."));
   *
   * @param   {boolean} unicode Flag specifying whether the string are using unicode encoding.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.PipeSetDefaultStringMode = function(unicode) {
    return this.SendRequest.call(this, 'PipeSetDefaultStringMode', [unicode]);
  };

  /**
   * Requests the number of bytes pending on the receive buffer.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeGetRxBytes(0).then(response => console.log(response.data));
   *
   * @param   {number} port Pipe ID.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number.
   */
  PCM.prototype.PipeGetRxBytes = function(port) {
    return this.SendRequest.call(this, 'PipeGetRxBytes', [port]);
  };

  /**
   * Requests the number of bytes pending on the send buffer.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeGetTxBytes(0).then(response => console.log(response.data));
   *
   * @param   {number} port Pipe ID.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number.
   */
  PCM.prototype.PipeGetTxBytes = function(port) {
    return this.SendRequest.call(this, 'PipeGetTxBytes', [port]);
  };

  /**
   * Requests the number of free bytes from the send buffer.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeGetTxFree(0).then(response => console.log(response.data));
   *
   * @param   {number} port Port number that identified the pipe.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number.
   */
  PCM.prototype.PipeGetTxFree = function(port) {
    return this.SendRequest.call(this, 'PipeGetTxFree', [port]);
  };

  /**
   * Requests the receive buffer size of a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeGetRxBufferSize(0).then(response => console.log(response.data));
   *
   * @param   {number} port Pipe ID.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number.
   */
  PCM.prototype.PipeGetRxBufferSize = function(port) {
    return this.SendRequest.call(this, 'PipeGetRxBufferSize', [port]);
  };

  /**
   * Requests the send buffer size of a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeGetTxBufferSize(0).then(response => console.log(response.data));
   *
   * @param   {number} port Pipe ID.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number.
   */
  PCM.prototype.PipeGetTxBufferSize = function(port) {
    return this.SendRequest.call(this, 'PipeGetTxBufferSize', [port]);
  };

  /**
   * Writes a string to a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeWriteString(0, "Hello world!", false, false).then(response => console.log(response.data));
   *
   * @param   {number}  port         Pipe ID.
   * @param   {string}  str          String to be written to the pipe.
   * @param   {boolean} allOrNothing Flag specifying whether the string should be sent all at once.
   * @param   {boolean} unicode      Flag specifying whether the string is unicode encoded.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written characters.
   */
  PCM.prototype.PipeWriteString = function(port, str, allOrNothing, unicode) {
    return this.SendRequest.call(this, 'PipeWriteString', [port, str, allOrNothing, unicode]);
  };

  /**
   * Writes an array of signed integers to a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeWriteIntArray(0, 2, [1, 2, 3, 4, 5], false).then(response => console.log(response.data));
   *
   * @param   {number}        port         Pipe ID.
   * @param   {number}        elSize       Element size, can be 1, 2, 4, or 8.
   * @param   {Array<number>} data         Array of integers to be written.
   * @param   {boolean}       allOrNothing Flag specifying whether the string should be sent all at once.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written array elements.
   */
  PCM.prototype.PipeWriteIntArray = function(port, elSize, data, allOrNothing) {
    return this.SendRequest.call(this, 'PipeWriteIntArray', [port, elSize, data, allOrNothing]);
  };

  /**
   * Writes an array of unsigned integers to a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeWriteUIntArray(0, 4, [100, 200, 300, 400, 500], false).then(response => console.log(response.data));
   *
   * @param   {number}        port         Pipe ID.
   * @param   {number}        elSize       Element size, can be 1, 2, 4, or 8.
   * @param   {Array<number>} data         Array of integers to be written.
   * @param   {boolean}       allOrNothing Flag specifying whether the string should be sent all at once.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written array elements.
   */
  PCM.prototype.PipeWriteUIntArray = function(port, elSize, data, allOrNothing) {
    return this.SendRequest.call(this, 'PipeWriteUIntArray', [port, elSize, data, allOrNothing]);
  };

  /**
   * Writes an array of floating-point numbers to a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeWriteFloatArray(0, [1.0, 2.0, 3.0, 4.0, 5.0], false).then(response => console.log(response.data));
   *
   * @param   {number}        port         Pipe ID.
   * @param   {Array<number>} data         Array of floating-point numbers to be written.
   * @param   {boolean}       allOrNothing Flag specifying whether the string should be sent all at once.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written array elements.
   */
  PCM.prototype.PipeWriteFloatArray = function(port, data, allOrNothing) {
    return this.SendRequest.call(this, 'PipeWriteFloatArray', [port, data, allOrNothing]);
  };

  /**
   * Writes an array of double-precision numbers to a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeWriteDoubleArray(0, [10.0, 20.0, 30.0, 40.0, 50.0], false).then(response => console.log(response.data));
   *
   * @param   {number}        port         Pipe ID.
   * @param   {Array<number>} data         Array of integers to be written.
   * @param   {boolean}       allOrNothing Flag specifying whether the string should be sent all at once.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of successfully written array elements.
   */
  PCM.prototype.PipeWriteDoubleArray = function(port, data, allOrNothing) {
    return this.SendRequest.call(this, 'PipeWriteDoubleArray', [port, data, allOrNothing]);
  };

  /**
   * Reads a string from a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeReadString(0, 100, 512, false, false).then(response => console.log(response.data));
   *
   * @param   {number}  port         Pipe ID.
   * @param   {number}  rxTimeout_ms Read timeout in milliseconds.
   * @param   {number}  charsToRead  Number of characters to read.
   * @param   {boolean} allOrNothing Flag specifying whether the string should be read all at once.
   * @param   {boolean} unicode      Flag specifying whether the string is unicode encoded.
   * @returns {Promise} In case of success, resolved promise will contain data property of type string.
   */
  PCM.prototype.PipeReadString = function(port, rxTimeout_ms, charsToRead, allOrNothing, unicode) {
    return this.SendRequest.call(this, 'PipeReadString', [port, rxTimeout_ms, charsToRead, allOrNothing, unicode]);
  };

  /**
   * Reads an array of signed integers from a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeReadIntArray(0, 2, 100, 100, false).then(response => console.log(response.data));
   *
   * @param   {number}  port         Pipe ID.
   * @param   {number}  elSize       Element size, can be 1, 2, 4, or 8.
   * @param   {number}  rxTimeout_ms Read timeout in milliseconds.
   * @param   {number}  size         The size of the array (number of elements).
   * @param   {boolean} allOrNothing Flag specifying whether the array should be read all at once.
   * @returns {Promise} In case of success, resolved promise will contain data property of type Array<number>.
   */
  PCM.prototype.PipeReadIntArray = function(port, elSize, rxTimeout_ms, size, allOrNothing) {
    return this.SendRequest.call(this, 'PipeReadIntArray', [port, elSize, rxTimeout_ms, size, allOrNothing]);
  };

  /**
   * Reads an array of unsigned integers from a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeReadUIntArray(0, 4, 100, 100, false).then(response => console.log(response.data));
   *
   * @param   {number}  port         Pipe ID.
   * @param   {number}  elSize       Element size, can be 1, 2, 4, or 8.
   * @param   {number}  rxTimeout_ms Read timeout in milliseconds.
   * @param   {number}  size         The size of the array (number of elements).
   * @param   {boolean} allOrNothing Flag specifying whether the array should be read all at once.
   * @returns {Promise} In case of success, resolved promise will contain data property of type Array<number>.
   */
  PCM.prototype.PipeReadUIntArray = function(port, elSize, rxTimeout_ms, size, allOrNothing) {
    return this.SendRequest.call(this, 'PipeReadUIntArray', [port, elSize, rxTimeout_ms, size, allOrNothing]);
  };

  /**
   * Reads an array of floating-point numbers from a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeReadFloatArray(0, 100, 100, false).then(response => console.log(response.data));
   *
   * @param   {number}  port         Pipe ID.
   * @param   {number}  rxTimeout_ms Read timeout in milliseconds.
   * @param   {number}  size         The size of the array (number of elements).
   * @param   {boolean} allOrNothing Flag specifying whether the array should be read all at once.
   * @returns {Promise} In case of success, resolved promise will contain data property of type Array<number>.
   */
  PCM.prototype.PipeReadFloatArray = function(port, rxTimeout_ms, size, allOrNothing) {
    return this.SendRequest.call(this, 'PipeReadFloatArray', [port, rxTimeout_ms, size, allOrNothing]);
  };

  /**
   * Reads an array of double-precision numbers from a pipe.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @example
   * pcm.PipeReadDoubleArray(0, 100, 100, false).then(response => console.log(response.data));
   *
   * @param   {number}  port         Pipe ID.
   * @param   {number}  rxTimeout_ms Read timeout in milliseconds.
   * @param   {number}  size         The size of the array (number of elements).
   * @param   {boolean} allOrNothing Flag specifying whether the array should be read all at once.
   * @returns {Promise} In case of success, resolved promise will contain data property of type Array<number>.
   */
  PCM.prototype.PipeReadDoubleArray = function(port, rxTimeout_ms, size, allOrNothing) {
    return this.SendRequest.call(this, 'PipeReadDoubleArray', [port, rxTimeout_ms, size, allOrNothing]);
  };

  /**
   * Opens a file on the machine the service is running one.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#LocalFileClose LocalFileClose}
   *
   * @example
   * pcm.LocalFileOpen("D:\\Temp\\temp.txt", "w+").then(() => console.log("File open.");
   *
   * @param   {string} file Path to the file.
   * @param   {string} mode NodeJS file system {@link https://nodejs.org/api/fs.html#fs_file_system_flags flags}.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing file descriptor.
   */
  PCM.prototype.LocalFileOpen = function(file, mode) {
    return this.SendRequest.call(this, 'LocalFileOpen', [file, mode]);
  };

  /**
   * Closes a file.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#LocalFileOpen LocalFileOpen}
   *
   * @example
   * let result = await pcm.LocalFileClose(3).then(() => console.log("File closed."));
   *
   * @param   {number} handle File descriptor returned by {@link PCM#LocalFileOpen LocalFileOpen}.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.LocalFileClose = function(handle) {
    return this.SendRequest.call(this, 'LocalFileClose', [handle]);
  };

  /**
   * Reads a string from an open file.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#LocalFileOpen LocalFileOpen}
   *
   * @example
   * pcm.LocalFileReadString(3, 255, false).then(response => console.log(response.data));
   *
   * @param   {number}  handle      File descriptor.
   * @param   {number}  charsToRead Numbers of characters to read.
   * @param   {boolean} unicode     Flag specifying whether the string is unicode encoded.
   * @returns {Promise} In case of success, resolved promise will contain data property of type string.
   */
  PCM.prototype.LocalFileReadString = function(handle, charsToRead, unicode) {
    return this.SendRequest.call(this, 'LocalFileReadString', [handle, charsToRead, unicode]);
  };

  /**
   * Writes a string to an open file.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &check;
   * @see {@link PCM#LocalFileOpen LocalFileOpen}
   *
   * @example
   * pcm.LocalFileWriteString(3, "Hello world!", false).then(response => console.log(response.data));
   *
   * @param   {number}  handle      File descriptor.
   * @param   {string}  str         String to write.
   * @param   {boolean} unicode     Flag specifying whether the string is unicode encoded.
   * @param   {number}  [size]      Optional length of the string to write. Writes the full 'str' length if undefined.
   * @returns {Promise} In case of success, resolved promise will contain data property of type number representing the number of written characters.
   */
  PCM.prototype.LocalFileWriteString = function(handle, str, unicode, size) {
    return this.SendRequest.call(this, 'LocalFileWriteString', [handle, str, unicode, size]);
  };

  /**
   * Enables communication library logger.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#LogDisable LogDisable}
   *
   * @example
   * pcm.LogEnable("Test logger", "Test logger.log").then(() => console.log("Logger enabled."));
   *
   * @param   {string}  name  Logger name.
   * @param   {string}  file  Logger file, if empty all the logs will be printed in standard output.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.LogEnable = function(name, file) {
    return this.SendRequest.call(this, 'LogEnable', [name, file]);
  };

  /**
   * Disables communication library logger.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @see {@link PCM#LogEnable LogEnable}
   *
   * @example
   * pcm.LogDisable().then(() => console.log("Logger disabled."));
   *
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.LogDisable = function() {
    return this.SendRequest.call(this, 'LogDisable');
  };

  /**
   * Sets logging pattern.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @example
   * pcm.LogSetPattern("[%Y-%m-%d %T.%f]: %v").then(() => console.log("Logger pattern updated."));
   *
   * @param   {string}  pattern  Logging pattern. Refer to [spdlog wiki]{@link https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags} for the list of available flags.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.LogSetPattern = function(pattern) {
    return this.SendRequest.call(this, 'LogSetPattern', [pattern]);
  };

  /**
   * Sets logging verbosity level.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @example
   * pcm.LogSetVerbosity(2).then(() => console.log("Verbosity level set to normal."));
   *
   * @param   {number}  verbosity  Logging verbosity level.
   *
   * | Level | Description |
   * | :---- | :---------- |
   * | 0     | OFF         |
   * | 1     | Minimal     |
   * | 2     | Normal      |
   * | 3     | Medium      |
   * | 4     | High        |
   * | 5     | All         |
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.LogSetVerbosity = function(verbosity) {
    return this.SendRequest.call(this, 'LogSetVerbosity', [verbosity]);
  };

  /**
   * Sets logging service filter.
   *
   * **Compatibility:** FreeMASTER &cross;, FreeMASTER Lite &check;
   * @example
   * // 0xFFFF - disables all current filters
   * // 0x0002 | 0x0004 - enables read and write filtering
   * pcm.LogSetServices(0xFFFF, 0x0002 | 0x0004).then(() => console.log("Filtering read and write services."));
   *
   * @param   {number}  mask      Mask that will disable current filtered services.
   * @param   {number}  services  Mask of services flags to be added to the filter.
   *
   * | Mask   | Service     |
   * | :----- | :---------- |
   * | 0x0001 | Port setup  |
   * | 0x0002 | Data Read   |
   * | 0x0004 | Data Write  |
   * | 0x0008 | Scope       |
   * | 0x0010 | Recorder    |
   * | 0x0020 | App command |
   * | 0x0040 | SFIO        |
   * | 0x0080 | TSA         |
   * | 0x0100 | PIPE        |
   * | 0x0200 | Polling     |
   * | 0xFFFF | All         |
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.LogSetServices = function(mask, services) {
    return this.SendRequest.call(this, 'LogSetServices', [mask, services]);
  };

  /**
   * Extends the object by extra methods and events available in full FreeMASTER UI application only.
   * Use this call in Control Pages and other scripts targeting the FreeMASTER application. Note that
   * the extra features will NOT work with FreeMASTER Lite service.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @deprecated this is a "dummy" method kept for backward compatibility
   */
  PCM.prototype.EnableExtraFeatures = function() {};

  /**
   * Start or stop the communication. This call is provided for backward compatibility with ActiveX
   * interface only. Use the StartComm and StopComm methods in new designs.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.StartStopComm(true).then(console.log("Communication port is open"));
   *
   * @param   {boolean} start    Start or stop communication.
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.StartStopComm = function(start) {
    return this.SendRequest.call(this, 'StartStopComm', [start]);
  };

  /**
   * Enable or disable events to be generated by the server side for this JSON-RPC session.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * function MyBoardDetectionHandler(name, id, value) {
   *   console.log("Board is detected");
   * }
   * pcm.OnBoardDetected = MyBoardDetectionHandler;
   * pcm.EnableEvents(true).then(console.log("Events are enabled"));
   *
   * @param   {boolean} enable      Enable or disable events.
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.EnableEvents = function(enable) {
    return this.SendRequest.call(this, 'EnableEvents', [enable]);
  };

  /**
   * Subscribe to variable changes at given testing period. When a variable is subscribed, FreeMASTER reads
   * the variable periodically and raises 'OnVariableChanged' event when a value change is detected. The events
   * need to be enabled by calling EnableEvents(true).
   * Event handler should have three parameters: 'name', 'id' and 'value'.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * function MyVariableChangedHandler(name, id, value) {
   *   console.log("Variable " + name + " has changed to value " + value);
   * }
   * pcm.OnVariableChanged = MyVariableChangedHandler;
   * pcm.SubscribeVariable(name).then(console.log("Variable is subscribed"));
   *
   * @param   {string}  name      Variable name.
   * @param   {number}  interval  Testing interval in milliseconds.
   * @returns {Promise} In case of success, resolved promise will contain 'xtra.subscriptionId' member which identifies the subscription.
   */
  PCM.prototype.SubscribeVariable = function(name, interval) {
    return this.SendRequest.call(this, 'SubscribeVariable', [name, interval]);
  };

  /**
    * Unsubscribe from variable changes subscribed previously with SubscribeVariable.
    *
    * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
    *
    * @example
    * pcm.UnsubscribeVariable(name).then(console.log("Variable is un-subscribed"));
    *
    * @param   {string}  name_or_id   Variable name or subscription identifier returned by previous SubscribeVariable call.
    * @returns {Promise} In case of success, resolved promise does not contain any data.
    */
  PCM.prototype.UnSubscribeVariable = function(name_or_id) {
    return this.SendRequest.call(this, 'UnSubscribeVariable', [name_or_id]);
  };

  /**
   * Define symbol.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.DefineSymbol(name, address, type, size).then(() => console.log("Symbol defined"));
   *
   * @param   {string}  name     Symbol name.
   * @param   {string}  address  Address.
   * @param   {string}  type     User type name (e.g. structure type name). Optional, leave empty for generic numeric types.
   * @param   {string}  [size]   Symbol size. Optional, leave empty to determine automatically when user type is specified.
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.DefineSymbol = function(name, address, type, size) {
    return this.SendRequest.call(this, 'DefineSymbol', [name, address, type, size]);
  };

  /**
   * Retrieve address and size of give symbol.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.GetSymbolInfo(name).then((response) => console.log("Symbol address=" + response.xtra.addr + " size=" + response.xtra.size));
   *
   * @param   {string}  name   Symbol name.
   * @returns {Promise} In case of success, resolved promise contains 'xtra' object with 'addr' and 'size' members.
   */
  PCM.prototype.GetSymbolInfo = function(name) {
    return this.SendRequest.call(this, 'GetSymbolInfo', [name]);
  };

  /**
   * Get structure or union member information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.GetStructMemberInfo(type, member).then((response) => console.log("Structure type " + type + " member " + member +
   *             " is at offset " + response.xtra.offset + ", size=" + response.xtra.size));
   *
   * @param   {string}  type    User type name.
   * @param   {string}  member  Structure member name.
   * @returns {Promise} In case of success, resolved promise contains 'xtra' object with 'offset' and 'size' members.
   */
  PCM.prototype.GetStructMemberInfo = function(type, member) {
    return this.SendRequest.call(this, 'GetStructMemberInfo', [type, member]);
  };

  /**
   * Delete all script-defined symbols.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.DeleteAllScriptSymbols().then(() => console.log("All script-defined symbols deleted."));
   *
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.DeleteAllScriptSymbols = function() {
    return this.SendRequest.call(this, 'DeleteAllScriptSymbols');
  };

  /**
   * Run variable stimulators.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.RunStimulators(name).then(() => console.log("Stimulator " + name + " is now running"));
   *
   * @param   {string}  name  Name of the variable stimulator to start (or more comma-separated names)
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.RunStimulators = function(name) {
    return this.SendRequest.call(this, 'RunStimulators', [name]);
  };

  /**
   * Stop variable stimulators.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.StopStimulators(name).then(() => console.log("Stimulator " + name + " is now stopped"));
   *
   * @param   {string}  name  Name of the variable stimulator to stop (or more comma-separated names)
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.StopStimulators = function(name) {
    return this.SendRequest.call(this, 'StopStimulators', [name]);
  };

  /**
   * Exit application.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.Exit();
   *
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.Exit = function() {
    return this.SendRequest.call(this, 'Exit');
  };

  /**
   * Activate FreeMASTER application window.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.ActivateWindow();
   *
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.ActivateWindow = function() {
    return this.SendRequest.call(this, 'ActivateWindow');
  };

  /**
   * Select item in FreeMASTER project tree and activate related view.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.SelectItem("My Oscilloscope", "osc");
   *
   * @param   {string}  name  Name of the item to activate
   * @param   {string}  tab   Tab to activate. Optional, one of the following values: "ctl", "blk", "info", "osc", "rec", "pipe"
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.SelectItem = function(name, tab) {
    return this.SendRequest.call(this, 'SelectItem', [name, tab]);
  };

  /**
   * Delete item in FreeMASTER project tree.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * {@link PCM#SelectItem SelectItem}
   *
   * @example
   * pcm.DeleteItem("My Oscilloscope");
   *
   * @param   {string}  name  Name or path of the item to delete.
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.DeleteItem = function(name) {
    return this.SendRequest.call(this, 'DeleteItem', [name]);
  };

  /**
   * Open specific FreeMASTER project.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.OpenProject("C:/projects/my_project.pmpx");
   *
   * @param   {string}  name  Name of the project file to open; use fully qualified name.
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.OpenProject = function(name) {
    return this.SendRequest.call(this, 'OpenProject', [name]);
  };

  /**
   * Save current FreeMASTER project.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.SaveProject(true);
   *
   * @param   {boolean|string} saveAs  When false, project is silently saved to last known path.
   *                            When true or when project is new a SaveAs dialog will open.
   *                            When a filename string is passed, the project is saved to the given file.
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.SaveProject = function(saveAs) {
    return this.SendRequest.call(this, 'SaveProject', [saveAs]);
  };

  /**
   * Determine if board has an active content defined in TSA table.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.IsBoardWithActiveContent().then((response) => console.log("Board active content " + (response.data ? "is" : "is NOT") + "present"));
   *
   * @returns {Promise} In case of success, resolved promise contains boolean 'data' member with return value.
   */
  PCM.prototype.IsBoardWithActiveContent = function() {
    return this.SendRequest.call(this, 'IsBoardWithActiveContent');
  };

  /**
   * Enumerate hyperlinks defined by active content.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * for(let index=0; true; index++) {
   *     try {
   *         let response = await pcm.EnumHrefLinks(index);
   *         console.log(response.xtra.name + " " +  response.xtra.retval + "\n");
   *     } catch (err) {
   *         break;
   *     }
   * }
   *
   * @returns {Promise} In case of success, resolved promise contains 'xtra' object with 'name' and 'retval' properties.
   */
  PCM.prototype.EnumHrefLinks = function(index) {
    return this.SendRequest.call(this, 'EnumHrefLinks', [index]);
  };

  /**
   * Enumerate project files defined by active content.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * for(let index=0; true; index++) {
   *     try {
   *         let response = await pcm.EnumProjectFiles(index);
   *         console.log(response.xtra.name + " " +  response.xtra.retval + "\n");
   *     } catch (err) {
   *         break;
   *     }
   * }
   *
   * @returns {Promise} In case of success, resolved promise contains 'xtra' object with 'name' and 'retval' properties.
   */
  PCM.prototype.EnumProjectFiles = function(index) {
    return this.SendRequest.call(this, 'EnumProjectFiles', [index]);
  };

  /**
   * Set global flag which affects Control Page reloading after opening port. By default, the page reloads when port is open.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.SetPageReloadOnPortOpen(false);
   *
   * @returns {Promise} In case of success, resolved promise does not contain any data.
   */
  PCM.prototype.SetPageReloadOnPortOpen = function(value) {
    return this.SendRequest.call(this, 'SetPageReloadOnPortOpen', [value]);
  };

  /**
   * Get global flag which affects Control Page reloading while opening port.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.GetPageReloadOnPortOpen().then(result => console.log("PageReloadOnPortOpen flag is " + result.data))
   *
   * @returns {Promise} In case of success, resolved promise contains boolean 'data' member with return value.
   */
  PCM.prototype.GetPageReloadOnPortOpen = function() {
    return this.SendRequest.call(this, 'GetPageReloadOnPortOpen');
  };

  /**
   * Get info about target memory address. Return name of symbol mapped to the address.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.GetAddressInfo(addr, size).then((response) => console.log("Symbol name is " + response.xtra.symbolName));
   *
   * @param   {number} addr Address of the variable.
   * @param   {number} size Size of the variable.
   * @returns {Promise} In case of success, resolved promise contains 'xtra.symbolName' and 'xtra.isExactMatch' values.
   */
  PCM.prototype.GetAddressInfo = function(addr, size) {
    return this.SendRequest.call(this, 'GetAddressInfo', [addr, size]);
  };

  /**
   * Create an Oscilloscope object in the project tree.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.DefineOscilloscope(name, defStr).then(() => console.log("Item created"));
   *
   * @param   {string} name Name of item.
   * @param   {string} defStr Stringified JSON definition record. Refer to FreeMASTER Desktop app. documentation for more details.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DefineOscilloscope = function(name, defStr) {
    return this.SendRequest.call(this, 'DefineOscilloscope', [name, defStr]);
  };

  /**
   * Request Oscilloscope object information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * @see {@link PCM#DefineOscilloscope DefineOscilloscope}
   *
   * @example
   *     pcm.GetOscilloscopeDefinition(name).then(response => {
   *         console.log(response.data);
   *     });
   *
   * @param   {string} name Scope name or path with name.
   * @returns {Promise} In case of success, resolved promise will contain 'xtra.def' property with definition object.
   *                    Refer to FreeMASTER Desktop app. documentation for more details.
   */
  PCM.prototype.GetOscilloscopeDefinition = function(name) {
    return this.SendRequest.call(this, 'GetOscilloscopeDefinition', [name]);
  };

  /**
   * Create a Recorder object in the project tree.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.DefineRecorder(name, defStr).then(() => console.log("Recorder project item created"));
   *
   * @param   {string} name Name of item.
   * @param   {string} defStr Stringified JSON definition record. Refer to FreeMASTER Desktop app. documentation for more details.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DefineRecorder = function(name, defStr) {
    return this.SendRequest.call(this, 'DefineRecorder', [name, defStr]);
  };

  /**
   * Request Recorder object information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * @see {@link PCM#DefineRecorder DefineRecorder}
   *
   * @example
   *     pcm.GetRecorderDefinition(name).then(response => {
   *         console.log(response.data);
   *     });
   *
   * @param   {string} name Recorder name or path with name.
   * @returns {Promise} In case of success, resolved promise will contain 'xtra.def' property with definition object.
   *                    Refer to FreeMASTER Desktop app. documentation for more details.
   */
  PCM.prototype.GetRecorderDefinition = function(name) {
    return this.SendRequest.call(this, 'GetRecorderDefinition', [name]);
  };

  /**
   * Create an Array Viewer object in the project tree.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.DefineArrayViewer(name, defStr).then(() => console.log("ArrayViewer project item created"));
   *
   * @param   {string} name Name of item.
   * @param   {string} defStr Stringified JSON definition record. Refer to FreeMASTER Desktop app. documentation for more details.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DefineArrayViewer = function(name, defStr) {
    return this.SendRequest.call(this, 'DefineArrayViewer', [name, defStr]);
  };

  /**
   * Request Array Viewer object information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * @see {@link PCM#DefineArrayViewer DefineArrayViewer}
   *
   * @example
   *     pcm.GetArrayViewerDefinition(name).then(response => {
   *         console.log(response.data);
   *     });
   *
   * @param   {string} name Array Viewer name or path with name.
   * @returns {Promise} In case of success, resolved promise will contain 'xtra.def' property with definition object.
   *                    Refer to FreeMASTER Desktop app. documentation for more details.
   */
  PCM.prototype.GetArrayViewerDefinition = function(name) {
    return this.SendRequest.call(this, 'GetArrayViewerDefinition', [name]);
  };

  /**
   * Create a Pipe object in the project tree.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.DefinePipe(name, defStr).then(() => console.log("Pipe project item created"));
   *
   * @param   {string} name Name of item.
   * @param   {string} defStr Stringified JSON definition record. Refer to FreeMASTER Desktop app. documentation for more details.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DefinePipe = function(name, defStr) {
    return this.SendRequest.call(this, 'DefinePipe', [name, defStr]);
  };

  /**
   * Request Pipe object information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * @see {@link PCM#DefinePipe DefinePipe}
   *
   * @example
   *     pcm.GetPipeDefinition(name).then(response => {
   *         console.log(response.data);
   *     });
   *
   * @param   {string} name Array Viewer name or path with name.
   * @returns {Promise} In case of success, resolved promise will contain 'xtra.def' property with definition object.
   *                    Refer to FreeMASTER Desktop app. documentation for more details.
   */
  PCM.prototype.GetPipeDefinition = function(name) {
    return this.SendRequest.call(this, 'GetPipeDefinition', [name]);
  };

  /**
   * Define Watch Block item in project.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.DefineWatchBlock(name, defStr).then(() => console.log("WatchBlock project item created"));
   *
   * @param   {string} name Name of item.
   * @param   {string} defStr Stringified JSON definition record. Refer to FreeMASTER documentation for more details.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DefineWatchBlock = function(name, defStr) {
    return this.SendRequest.call(this, 'DefineWatchBlock', [name, defStr]);
  };

  /**
   * Requests Watch Block information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * @see {@link PCM#DefineWatchBlock DefineWatchBlock}
   *
   * @example
   *     pcm.GetWatchBlockDefinition(name).then(response => {
   *         console.log(response.data);
   *     });
   *
   * @param   {string} name Watch Block name or path with name.
   * @returns {Promise} In case of success, resolved promise will contain 'xtra.def' property with definition object.
   *                    Refer to FreeMASTER Desktop app. documentation for more details.
   */
  PCM.prototype.GetWatchBlockDefinition = function(name) {
    return this.SendRequest.call(this, 'GetWatchBlockDefinition', [name]);
  };

  /**
   * Send application command.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.SendCommand(send).then((response) => console.log("Command response code " + response.xtra.retCode));
   *
   * @param   {string}  send Application command.
   * @param   {boolean} wait Set true to wait for the command processing to finish.
   * @returns {Promise} In case of success, resolved promise contains string 'xtra.message' member with return message and 'xtra.retCode' member with command result code.
   */
  PCM.prototype.SendCommand = function(send, wait) {
    return this.SendRequest.call(this, 'SendCommand', [send, wait]);
  };

  /**
   * Get current recorder state.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.GetCurrentRecorderState().then((response) => console.log("Return is " + response.xtra.state));
   *
   * @returns {Promise} In case of success, resolved promise contains string 'xtra.state' member with return state.
   */
  PCM.prototype.GetCurrentRecorderState = function() {
    return this.SendRequest.call(this, 'GetCurrentRecorderState');
  };

  /**
   * Get current recorder data.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.GetCurrentRecorderData().then((response) => console.log("Recorder data: " + JSON.stringify(response.xtra.data)));
   *
   * @returns {Promise} In case of success, resolved promise contains string 'xtra.data' member with return data array-of-arrays.
   */
  PCM.prototype.GetCurrentRecorderData = function() {
    return this.SendRequest.call(this, 'GetCurrentRecorderData');
  };

  /**
   * Get current recorder series.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.GetCurrentRecorderSeries(name).then((response) => console.log("Recorder series data: " + JSON.stringify(response.xtra.data)));
   *
   * @param   {string} name Variable name.
   * @returns {Promise} In case of success, resolved promise contains string 'xtra.data' member with return data array.
   */
  PCM.prototype.GetCurrentRecorderSeries = function(name) {
    return this.SendRequest.call(this, 'GetCurrentRecorderSeries', [name]);
  };

  /**
   * Set global project options.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.SetProjectOptions(defStr, refreshViews).then(() => console.log("Project options set"));
   *
   * @param   {string} defStr Stringified JSON ProjectOptions record. Refer to FreeMASTER Desktop app. documentation for more details.
   * @param   {boolean} refreshViews When true, all views are fully refreshed after options are set (default false).
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.SetProjectOptions = function(defStr, refreshViews) {
    return this.SendRequest.call(this, 'SetProjectOptions', [defStr, refreshViews]);
  };

  /**
   * Get global project options.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * @see {@link PCM#SetProjectOptions SetProjectOptions}
   *
   * @example
   *     pcm.GetProjectOptions().then(response => {
   *         console.log("Project options: " + response.xtra.retval);
   *     });
   *
   * @returns {Promise} In case of success, resolved promise will contain 'xtra.retval' property with a stringified ProjectOptions object.
   *                    Refer to FreeMASTER Desktop app. documentation for more details.
   */
  PCM.prototype.GetProjectOptions = function() {
    return this.SendRequest.call(this, 'GetProjectOptions', []);
  };

  /**
   * Fire an event to all attached clients.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * @see {@link PCM#EnableEvents EnableEvents}
   *
   * @example
   * pcm.EnableEvents(true);
   * pcm.OnCustomEvent = function(arg) { console.log("Custom event received with argument " + JSON.stringify(arg)); }
   * pcm.FireCustomEvent("text message");
   *
   * @param   {string} arg String argument which will be passed back to an event handler
   * @returns {Promise} In case of success, resolved promise contains string 'xtra.data' member with return state.
   */
  PCM.prototype.FireCustomEvent = function(arg) {
    return this.SendRequest.call(this, 'FireCustomEvent', [arg]);
  };

  if (typeof define == 'function' && define.amd) {
    define('PCM', [], function() {
      return PCM;
    });
  } else if (typeof module !== 'undefined' && typeof module.exports !== 'undefined') {
    module.exports = PCM;
  } else if (typeof root !== 'undefined') {
    root.PCM = PCM;
  } else {
    return PCM;
  }

  /**
   * Create a Stimulator object in the project tree.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.DefineStimulator(name, defStr).then(() => console.log("Stimulator project item created"));
   *
   * @param   {string} name Name of stimulator.
   * @param   {string} defStr Stringified JSON definition record. Refer to FreeMASTER Desktop app. documentation for more details.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DefineStimulator = function(name, defStr) {
    return this.SendRequest.call(this, 'DefineStimulator', [name, defStr]);
  };

  /**
   * Request Stimulator object information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * @see {@link PCM#DefineStimulator DefineStimulator}
   *
   * @example
   *     pcm.GetStimulatorDefinition(name).then(response => {
   *         console.log(response.data);
   *     });
   *
   * @param   {string} name Name of stimulator.
   * @returns {Promise} In case of success, resolved promise will contain 'xtra.def' property with definition object.
   *                    Refer to FreeMASTER Desktop app. documentation for more details.
   */
  PCM.prototype.GetStimulatorDefinition = function(name) {
    return this.SendRequest.call(this, 'GetStimulatorDefinition', [name]);
  };

  /**
   * Create an Enum object in the project.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   *
   * @example
   * pcm.DefineEnum(name, defStr).then(() => console.log("Enum item created"));
   *
   * @param   {string} name Name of Enum.
   * @param   {string} defStr Stringified JSON definition record. Refer to FreeMASTER Desktop app. documentation for more details.
   * @returns {Promise} The result does not carry any relevant data.
   */
  PCM.prototype.DefineEnum = function(name, defStr) {
    return this.SendRequest.call(this, 'DefineEnum', [name, defStr]);
  };

  /**
   * Request Enum object information.
   *
   * **Compatibility:** FreeMASTER &check;, FreeMASTER Lite &cross;
   * @see {@link PCM#DefineEnum DefineEnum}
   *
   * @example
   *     pcm.GetEnumDefinition(name).then(response => {
   *         console.log(JSON.stringify(response.xtra.def));
   *     });
   *
   * @param   {string} name Name of Enum.
   * @returns {Promise} In case of success, resolved promise will contain 'xtra.def' property with definition object.
   *                    Refer to FreeMASTER Desktop app. documentation for more details.
   */
  PCM.prototype.GetEnumDefinition = function(name) {
    return this.SendRequest.call(this, 'GetEnumDefinition', [name]);
  };

})(this);
