--------------------------------THÔNG TIN SINH VIÊN--------------------------------
NGUYỄN THANH TRÍ
1512605
tringuyen3297@gmail.com


----------------------------------CHỨC NĂNG HOÀN THÀNH----------------------------------
1. Sử dụng shell để lấy các đối tượng thư mục và tập tin của các ổ đĩa.

2. Cải tiến cho phép khi thay đổi kích thước treeview thì listview thay đổi theo.
(Khi kéo các handle còn lại bị ảnh hưởng nhưng khi thả ra thì bình thường, bug chưa fix được)

3. Bổ  sung statusbar khi click vào một tập tin trong listview thì hiển thị kích thước 
tập tin tương ứng.

4. Sử dụng file config.ini để lưu lại kích thước cửa sổ màn hình chính và nạp lại khi 
chương trình chạy lên.



----------------------------------LUỒNG SỰ KIỆN----------------------------------
A.CHÍNH
    + Chạy chương trình, hiển thị node This PC trên TreeView bên trái ở trạng thái 
collapse (thu gọn). Bấm vào sẽ xổ xuống các node con là danh sách ổ đĩa.
    + Bấm đôi vào ổ đĩa bất kỳ đang ở trạng thái collapse(thu gọn) trong TreeView 
bên trái sẽ xổ xuống danh sách các thư mục con.
    + Bấm đôi vào thư mục bất kỳ đang ở trạng thái collapse(thu gọn) trong TreeView 
bên trái sẽ xổ xuống danh sách các thư mục con(nếu có).
    + Bấm đôi vào ổ đĩa/thư mục bất kì đang ở trạng thái expand(mở rộng) trong TreeView 
bên trái sẽ đóng lại danh sách các thư mục con tương ứng.
    + Bấm vào 1 thư mục/tập tin bất kì bên list view sẽ hiên thông tin trên status bar,
nếu là tập tin sẽ có thêm kích thước tập tin trên status bar.

B.PHỤ
    + Ở Treeview, nút Expand/Collapse của các ổ đĩa/thư mục chỉ được hiển thị khi
người dùng load qua mục đó 1 lần.

