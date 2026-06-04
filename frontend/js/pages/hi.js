const PageHi = {
  render() {
    const container = document.getElementById('page-hi');
    container.innerHTML = `
      <div class="body-header" style="text-align:center">
        <h1 class="text-title">Welcome!</h1>
      </div>
      <div style="text-align:center;padding:24px 0">
        <span class="icon icon-smile" style="font-size:72px;color:#0078D4"></span>
        <h2 class="text-subheader" style="margin-top:16px">Hi there, welcome back</h2>
      </div>
    `;
  }
};
