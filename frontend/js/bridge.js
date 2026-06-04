const Bridge = {
  pendingRequests: {},
  requestIdCounter: 0,

  send(command, payload = '') {
    return new Promise((resolve, reject) => {
      const requestId = 'req_' + (++this.requestIdCounter);
      this.pendingRequests[requestId] = { resolve, reject };

      const message = JSON.stringify({
        command,
        payload,
        requestId
      });

      try {
        if (window.chrome && window.chrome.webview) {
          window.chrome.webview.postMessage(message);
        } else {
          console.warn('Bridge: No WebView2 host available');
          setTimeout(() => {
            this.resolveMock(requestId, { command, payload });
          }, 100);
        }
      } catch (err) {
        delete this.pendingRequests[requestId];
        reject(err);
      }
    });
  },

  resolveMock(requestId, msg) {
    const pending = this.pendingRequests[requestId];
    if (pending) {
      pending.resolve({ success: true, data: {} });
      delete this.pendingRequests[requestId];
    }
  },

  receive(messageJson) {
    try {
      const response = JSON.parse(messageJson);
      const { requestId, success, data } = response;
      const pending = this.pendingRequests[requestId];
      if (pending) {
        if (success) {
          pending.resolve(response);
        } else {
          pending.reject(new Error(data || 'Unknown error'));
        }
        delete this.pendingRequests[requestId];
      }
    } catch (err) {
      console.error('Bridge: Failed to parse message:', err);
    }
  },

  // Convenience methods
  navigateTo(page) { return this.send('navigateTo', page); },
  goBack() { return this.send('goBack'); },
  goNext() { return this.send('goNext'); },
  cancel() { return this.send('cancel'); },
  reset() { return this.send('reset'); },
  setRegion(region) { return this.send('setRegion', region); },
  setKeyboardLayout(layout) { return this.send('setKeyboardLayout', layout); },
  setKeyboardLayoutSecondary(layout) { return this.send('setKeyboardLayoutSecondary', layout); },
  setNetworkConfig(config) { return this.send('setNetworkConfig', JSON.stringify(config)); },
  acceptLicense() { return this.send('acceptLicense'); },
  setDeviceName(name) { return this.send('setDeviceName', name); },
  setAccountInfo(info) { return this.send('setAccountInfo', JSON.stringify(info)); },
  setPassword() { return this.send('setPassword'); },
  setPrivacySettings(settings) { return this.send('setPrivacySettings', JSON.stringify(settings)); },
  setCustomization(settings) { return this.send('setCustomization', JSON.stringify(settings)); },
  submitAll() { return this.send('submitAll'); },
  getState() { return this.send('getState'); },
  exportState() { return this.send('exportState'); },
  importState() { return this.send('importState'); },
  getLogs() { return this.send('getLogs'); },
  enableDevMode() { return this.send('enableDevMode'); },
  disableDevMode() { return this.send('disableDevMode'); },
  skipToPage(page) { return this.send('skipToPage', page); },
  getWallpaper() { return this.send('getWallpaper'); }
};

if (window.chrome && window.chrome.webview) {
  window.chrome.webview.addEventListener('message', (event) => {
    Bridge.receive(event.data);
  });
}

document.addEventListener('DOMContentLoaded', () => {
  console.log('Bridge initialized');
});
