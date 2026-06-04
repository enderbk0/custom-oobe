const PageAccount = {
  render() {
    const container = document.getElementById('page-account');
    container.innerHTML = `
      <h1>Who's going to use this device?</h1>
      <p class="subtitle">Enter a name for the person who'll use this device.</p>
      <div class="content-area">
        <div class="form-group">
          <label class="form-label" for="account-name">Name</label>
          <input type="text" id="account-name" class="form-input"
                 placeholder="Enter your name" maxlength="20"
                 autocomplete="off" spellcheck="false">
        </div>
        <p class="subtitle" style="font-size:var(--font-size-sm)">
          This will be your local account on this device.
        </p>
      </div>
    `;

    const input = document.getElementById('account-name');
    input.addEventListener('input', () => {
      ValidationUI.clearError('account-name');
    });

    input.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') {
        e.preventDefault();
        document.getElementById('btn-next').click();
      }
    });

    const saved = AppState.get('accountName');
    if (saved) input.value = saved;

    Router.registerValidator('account', () => {
      const value = input.value.trim();
      const error = Validators.username(value);
      if (error) {
        ValidationUI.showError('account-name', error);
        return false;
      }
      AppState.set('accountName', value);
      Bridge.setAccountInfo({ name: value });
      return true;
    });
  }
};
