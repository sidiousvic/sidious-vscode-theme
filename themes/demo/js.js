function Enemy(x, y, img, width, height) {
    // assign this to a variable to always refer to this object
    // through closure regardless of execution context
    let _this = this;
  
    this.x = x;
    this.y = y;
    this.img = img;
    this.width = width;
    this.height = height;
    this.velocity = {
      x: -2,
      y: -2
    };
  
    this.update = function(enemies) {
      _this.draw();
  
      for (let i = 0; i < enemies.length; i++) {
        if (_this === enemies[i]) continue;
        if (distance(_this.x, _this.y, enemies[i].x, enemies[i].y) - 20 < 0) {
          // HITBOX ^^^
          _this.velocity.x = -_this.velocity.x;
          _this.velocity.y = -_this.velocity.y;
          if (_this.img === document.getElementById('enemy')) {
            _this.img = document.getElementById('enemyR');
          } else {
            _this.img = document.getElementById('enemy');
          }
        }
