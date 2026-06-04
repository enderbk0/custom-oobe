const AppState = {
  _data: {},

  init() {
    const saved = localStorage.getItem('oobe_state');
    if (saved) {
      try {
        this._data = JSON.parse(saved);
      } catch (e) {
        this._data = {};
      }
    }
  },

  get(key, defaultValue = null) {
    return this._data.hasOwnProperty(key) ? this._data[key] : defaultValue;
  },

  set(key, value) {
    this._data[key] = value;
    this._persist();
    this._notify(key, value);
  },

  setMultiple(entries) {
    Object.entries(entries).forEach(([key, value]) => {
      this._data[key] = value;
    });
    this._persist();
  },

  getAll() {
    return { ...this._data };
  },

  reset() {
    this._data = {};
    this._persist();
  },

  _persist() {
    try {
      localStorage.setItem('oobe_state', JSON.stringify(this._data));
    } catch (e) {
      console.warn('State: Failed to persist', e);
    }
  },

  _listeners: {},

  onChange(key, callback) {
    if (!this._listeners[key]) {
      this._listeners[key] = [];
    }
    this._listeners[key].push(callback);
  },

  _notify(key, value) {
    const listeners = this._listeners[key] || [];
    listeners.forEach(cb => cb(value));
  }
};

AppState.init();
