const serviceAddon = require("./build/Release/service");
const remoteAddon = require("./build/Release/remote");

function executeCommand(serviceName, cmd) {
  const res = serviceAddon.execute_command(serviceName, cmd);

  if (res === -1) {
    throw "could not open service manager";
  } else if (res === -2) {
    throw `could not connect to service ${serviceName}`;
  } else if (res === -3) {
    throw `sending command ${cmd} to service ${serviceName} failed`;
  } else if (res > 0) {
    throw `sending command ${cmd} to service ${serviceName} failed with error code ${res}`;
  }
}

function isCurrentSessionRemoteable(serviceName, cmd) {
  const res = remoteAddon.is_current_session_remoteable();
  return res === 1 ? true : false;
}

module.exports.executeCommand = executeCommand;
module.exports.isCurrentSessionRemoteable = isCurrentSessionRemoteable;
