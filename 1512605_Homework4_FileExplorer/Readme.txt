--------------------------------THÔNG TIN SINH VIÊN--------------------------------
NGUYỄN THANH TRÍ
1512605
tringuyen3297@gmail.com


----------------------------------CHỨC NĂNG HOÀN THÀNH----------------------------------
A.Chức năng cơ bản
    + Tạo ra TreeView bên trái, ListView bên phải. 
    + Xét TreeView
	   - Tạo node root là This PC.
	   - Lấy danh sách các ổ đĩa trong máy.
	   - Lấy các icon từ ảnh bitmap có sẵn.
	   - Bắt sự kiện Expanding, lấy ra đường dẫn dấu ở PARAM để biết mình 
	phải xư lí thư mục nào, duyệt nội dung thư mục bằng FindFirstFile & 
	FindNextFile, chỉ lấy các thư mục để thêm vào làm node con.
    + Xét ListView
	   -Hiển thị toàn bộ thư mục và tập tin tương ứng với một đường dẫn.
	   -Bấm đôi vào một thư mục sẽ thấy toàn bộ thư mục con và tập tin.
	   -Tạo ra ListView có 4 cột: Tên, Loại, Thời gian chỉnh sửa, Dung lượng. 
	   -Lấy các icon từ ảnh bitmap có sẵn.

B.Chức năng nâng cao
    + Bấm đôi 1 tập tin sẽ chạy app tương ứng (vd:docx mở Word, pptx mở Powerpoint, 
txt mở Notepad,...)
    + Ở Listview, đọc được loại tập tin để hiển thị lên cột "loại tập tin" (trừ những 
đuôi lạ cả hệ thống cũng không biết).


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

B.PHỤ
    + Ở Treeview, nút Expand/Collapse của các ổ đĩa/thư mục chỉ được hiển thị khi
người dùng load qua mục đó 1 lần.


---------------------------------------LINK--------------------------------------
source: https://thanhtri3297@bitbucket.org/thanhtri3297/1512605_explorer.git
video: https://youtu.be/oxR6-X2aycc