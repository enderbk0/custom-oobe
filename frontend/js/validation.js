const Validators = {
  required(value) {
    return value && value.trim().length > 0 ? null : 'This field is required';
  },

  minLength(min) {
    return function(value) {
      return value && value.length >= min ? null : `Must be at least ${min} characters`;
    };
  },

  maxLength(max) {
    return function(value) {
      return value && value.length <= max ? null : `Must be no more than ${max} characters`;
    };
  },

  pattern(regex, message) {
    return function(value) {
      return regex.test(value) ? null : (message || 'Invalid format');
    };
  },

  matches(otherValue, message) {
    return function(value) {
      return value === otherValue ? null : (message || 'Values do not match');
    };
  },

  deviceName(value) {
    if (!value || value.trim().length === 0) return 'Enter a name for this device';
    if (value.length < 1) return 'Name must be at least 1 character';
    if (value.length > 15) return 'Name must be 15 characters or fewer';
    if (!/^[a-zA-Z0-9-]+$/.test(value)) return 'Use only letters, numbers, and hyphens';
    return null;
  },

  username(value) {
    if (!value || value.trim().length === 0) return 'Enter a name';
    if (value.length < 1) return 'Name must be at least 1 character';
    if (value.length > 20) return 'Name must be 20 characters or fewer';
    if (!/^[a-zA-Z0-9_ .-]+$/.test(value)) return 'Use only letters, numbers, periods, and spaces';
    return null;
  },

  password(value) {
    if (!value || value.length === 0) return 'Enter a password';
    if (value.length < 8) return 'Password must be at least 8 characters';
    if (value.length > 127) return 'Password must be 127 characters or fewer';
    return null;
  },

  validateField(value, validators) {
    for (const validator of validators) {
      const error = validator(value);
      if (error) return error;
    }
    return null;
  }
};

const ValidationUI = {
  showError(elementId, message) {
    const el = document.getElementById(elementId);
    if (el) {
      el.classList.add('error');
    }
    let errorEl = document.getElementById(elementId + '-error');
    if (!errorEl) {
      errorEl = document.createElement('div');
      errorEl.id = elementId + '-error';
      errorEl.className = 'form-error';
      if (el && el.parentNode) {
        el.parentNode.appendChild(errorEl);
      }
    }
    errorEl.textContent = message;
  },

  clearError(elementId) {
    const el = document.getElementById(elementId);
    if (el) {
      el.classList.remove('error');
    }
    const errorEl = document.getElementById(elementId + '-error');
    if (errorEl) {
      errorEl.textContent = '';
    }
  },

  clearAll() {
    document.querySelectorAll('.form-error').forEach(el => el.textContent = '');
    document.querySelectorAll('.error').forEach(el => el.classList.remove('error'));
  }
};
