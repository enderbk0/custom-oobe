const PagePreparing = {
  render() {
    const container = document.getElementById('page-preparing');
    container.innerHTML = `
      <div class="body-header" style="text-align:center">
        <h1 class="text-title">Preparing Windows</h1>
      </div>
      <div class="control-progress" style="text-align:center;padding:24px 0">
        <progress class="win-ring win-large"></progress>
        <h4 class="preparing-status">Applying your settings...</h4>
      </div>
    `;
  }
};
