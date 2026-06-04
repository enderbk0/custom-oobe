const PageCompletion = {
  render() {
    const container = document.getElementById('page-completion');
    container.innerHTML = `
      <div class="body-header" style="text-align:center">
        <h1 class="text-title">You're all set!</h1>
      </div>
      <div style="text-align:center;padding:24px 0">
        <span class="icon icon-verify" style="font-size:72px;color:#0078D4;display:inline-block"></span>
        <h2 class="text-subheader" style="margin-top:16px">Your device is ready to use</h2>
      </div>
    `;
  }
};
