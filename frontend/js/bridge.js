const Bridge = {
  _data: {},

  setRegion(code) { this._data.region = code; },
  setKeyboardLayout(code) { this._data.keyboard = code; },
  setKeyboardSecondaryLayout(code) { this._data.keyboardSecondary = code; },
  setNetwork(ssid) { this._data.network = ssid; },
  setPassword() { this._data.password = true; },
  setDeviceName(name) { this._data.deviceName = name; },
  setAccountName(name) { this._data.accountName = name; },
  setPrivacySettings(s) { this._data.privacy = s; },

  async submitAll() {
    console.log('Bridge: Submitting settings:', this._data);
    return true;
  }
};
