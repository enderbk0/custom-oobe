const PageAccount = {
  render() {
    const container = document.getElementById('page-account');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Who's going to use this device?</h1>
      </div>
      <p class="content-lead">Enter the name for the account you want to create.</p>
      <fieldset>
        <legend>Account name</legend>
        <div class="template-input">
          <input type="text" id="account-name" class="win-textbox win-textbox-PuaCompatible-font"
                 placeholder="Enter a name" autofocus />
        </div>
      </fieldset>
    `;

    const input = document.getElementById('account-name');
    input.addEventListener('input', () => {
      AppState.set('accountName', input.value);
      Bridge.setAccountName(input.value);
    });
    input.addEventListener('keydown', (e) => {
      if (e.key === 'Enter') {
        e.preventDefault();
        document.getElementById('btn-next').click();
      }
    });

    const saved = AppState.get('accountName');
    if (saved) input.value = saved;
  },

  onBeforeNext() {
    const name = AppState.get('accountName');
    if (!name || name.trim().length === 0) return false;
    return true;
  }
};
