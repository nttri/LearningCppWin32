-----------------THÔNG TIN CÁ NHÂN-------------------
Mssv:	1512605
Họ tên:	Nguyễn Thanh Trí

--------------CHỨC NĂNG ĐÃ HOÀN THÀNH----------------
CƠ BẢN:
   1. Tạo ra giao diện có một edit control để nhập liệu.
   2. Lưu lại dưới dạng file text.
   3. Mở một tập tin, đọc nội dung và hiển thị để chỉnh sửa tiếp.
   4. Hỗ trợ 3 thao tác Cut - Copy - Paste.
NÂNG CAO:
   1. Hỗ trợ mở file bằng OpenFileDialog.
   2. Hỗ trợ lưu file bằng SaveFileDialog.
   3. Hỗ trợ thao tác trên file định dạng UTF-8 (chưa hỗ trợ cho Unicode/UTF-16).

-------------------LUỒNG SỰ KIỆN---------------------
CHÍNH:
   Trong SaveFileDialog, nhập tên file rồi nhấn Save (có đuôi '.txt' hay không đều được).
PHỤ:
   Nếu tên file chứa đuôi khác '.txt' sẽ báo lỗi.

-------------------LINK BITBUCKET--------------------
git clone https://thanhtri3297@bitbucket.org/thanhtri3297/windev_1512605_notepad.git