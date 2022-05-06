/*global describe,it*/

var assert = require("assert");
var lib = require("../");

describe("ExecuteCommand", function () {
  it("should receive command", function () {
    // TODO: start udater service
    lib.executeCommand("CresusUpdaterService", 200); // CheckAvailable command
  });

  it("should not connect to service", function () {
    assert.throws(() => lib.executeCommand("DummyService", 200));
  });
});

describe("IsCurrentSessionRemoteable", function () {
  it("should not be remoteable", function () {
    const isRemoteable = lib.isCurrentSessionRemoteable();
    assert.equal(isRemoteable, false);
  });
});
