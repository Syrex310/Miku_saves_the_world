Mã số Sinh viên: 24020102.
Họ và Tên: Ngô Phú Quang Dương
Lớp Môn học: INT 2215 - 3
1. Thông tin về game: link (github, google doc, youtube), game tham khảo (link, miêu tả) 
- Link github: <https://github.com/Syrex310/Miku_saves_the_world>
- Youtube: <https://youtu.be/n1KNwVAwH04?si=_HrzXmK6PahClzYO>
- Game tham khảo: <https://store.steampowered.com/app/2420510/HoloCure__Save_the_Fans/>
2. Các tính năng cải tiến trong game
- Thêm menu nâng cấp nhân vật: health, attack, speed, fire rate, crir rate.
- Tăng độ khó sau từng wave(max là 10 waves) ở health và speed của enemy, tại wave 5 và 10 sẽ có boss(width và height to hơn, speed chậm hơn, health \*= 50\*wave)
3. Cấu trúc của project game: Tổ chức lớp, Miêu tả luồng, …
- Tổ chức lớp gồm: Player, Enemy, Bullet
  - Player: tọa độ(x, y); width(chiều rộng), height(chiều cao), speed(tốc độ chạy), health(máu hiện tại), maxhealth(máu tối đa), lastDamageTime(thời gian vừa bị mất máu)(giúp tính khoảng cách giữa những lần mất máu / frame).
  - Enemy: tọa độ(x,y); width(chiều rộng), height(chiều cao), speed(tốc độ chạy), health(máu hiện tại), hàm void *moveTowardPlayer*: di chuyển về phía player.
  - Bullet: tọa độ(x, y); speed(tốc độ đạn); (speedX, speedY) tốc độ di chuyển về phía Ox/Oy để về sau tính tốc độ thực khi di chuyển ở cả 2 trục. Hàm void *move* di chuyển viên đạn.
- Miêu tả luồng:
  - Khởi tạo game: gồm hàm *init* để debug, hàm *initializeGame* để khởi tạo fonts, textures, sounds và load dữ liệu từ save.txt
  - Vòng lặp: gameLoop
    - Xử lý event: *handleMenuInput* để xử lý ở menu chính, menu upgrade; *handleGameInput* xử lý trong game, pausegame, death/win screen.
    - Cập nhật logic game: kiểm tra va chạm: *updateCollisions*; di chuyển player: *updatePlayerMovement*; update đường đạn: *updateShooting*. Cập nhật wave mới: *spawnWaves*;….. Đầu và cuối có hàm giới hạn fps = 60.
    - Render: Vẽ từ background -> player -> gun -> enemy -> bullet -> currency(tiền tệ) -> health -> remainhealth -> ingame hp bar. Sau đó là check wave có đang chạy không để đưa ra thông báo: Wave mới + số lượng enemies.
    - Save game(*gameSave*) sẽ xảy ra khi: nâng cấp player, quit game, pause game, cập nhật file *save.txt* nếu bị lỗi.
  - Kết thúc game: Kiểm tra điều kiện dead hoặc win => hiển thị màn hình tùy trường hợp. Ngoài ra có thể pause game và chọn [resume / restart / menu / exit].
- Các file cpp/hpp của game:
  - Main.cpp: vòng lặp chính
  - Game.cpp: quản lý logic + gameState + gọi hàm
  - Common.hpp: khai báo biến, hằng số, thư viện, hàm,…
  - Bullet.cpp: Quản lý đạn + di chuyển, xử lý event *handleGameInput*
  - Player.cpp: Quản lý nhân vật + di chuyển
  - Gun.cpp: Quản lý gun + hướng xoay
  - Gif.cpp: Render gif sau menu upgrade
  - Menu.cpp: Render menu, xử lý event *handleMenuInput* ở đây
  - Save.cpp: Nâng cấp nhân vật, đọc và ghi save file *save.txt*
  - Enemy.cpp: Quản lý enemy + di chuyển + wave spawn và boss
  - Collision.cpp: Quản lý va chạm và xóa bỏ bullet/enemy khi hit/chết
4. Nguồn tham khảo và lấy dữ liệu: âm thanh, đồ họa, chatgpt, 
- Hình ảnh như: menu, menu upgrade, menu pause, nhân vật, súng, ingame,… là tự vẽ(có file .kra chứng minh)
- Âm thanh:
  - AudioCoffee Band - Pure Inspiration. Source: Free Music Archive.
  - AudioCoffee Band - Digital Technology. Source: Free Music Archive.
  - Triple5 Here – Crystallogy. Source: Free Music Archive.
  - Các sound effect khác(tiếng súng, mèo kêu, click chuột) free ở Pixabay.
  - Link: <https://pixabay.com>, <https://freemusicarchive.org/home>
- Chatgpt: Nhờ fix lỗi, tìm thông tin hàm, optimize code.
- SDL2 documentation: thông tin về SDL mixer, ttf, image.
- Stack Overflow: Một số cách xây dựng code.
5. Các thông tin khác
- Hướng dẫn chơi:
  - Di chuyển: W A S D
  - Bắn: Giữ chuột trái
  - Nâng cấp: Từ MENU -> STAT
  - Mục tiêu: Tiêu diệt kẻ địch và boss, sống sót 10 waves.
- Đồ họa + gameplay:
  - Lấy cảm hứng từ Holocure, đồ họa “pixel” tự vẽ.
  - Hiệu ứng âm thanh xử lý bằng SDL2
- Tips + thông tin thêm:
  - Player có chỉ số cơ bản: HP = 100, ATK = 25, SPEED = 3, FIRE RATE = 1 tương đương 4 RPS (rounds per second), CRIT RATE = 0, CRIT DAMAGE = 2.
  - Enemy có chỉ số cơ bản: HP = 50, ATK = 10, SPEED = 2
  - Mỗi wave enemy sẽ tăng: HP + 25, SPEED + 1 mỗi 3 waves(4, 7, 10)
