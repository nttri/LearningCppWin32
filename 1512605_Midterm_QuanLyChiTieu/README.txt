--------------------------------THÔNG TIN SINH VIÊN--------------------------------
NGUYỄN THANH TRÍ
1512605
tringuyen3297@gmail.com


-------------------------------CHỨC NĂNG HOÀN THÀNH--------------------------------
1.Thêm vào danh sách một chi tiêu (gồm 3 thành phần: loại, nội dung, chi phí).

2.Hiển thị các chi tiêu lên danh sách (dùng LISTVIEW) khi người dùng thêm mới hay
khi load dữ liệu từ tập tin khi mở ứng dụng (nếu có dữ liệu).

3.Tự động lưu dữ liệu xuống file khi người dùng thoát.

4.Vẽ biểu đồ tròn, có chú thích (màu-loại-tỉ lệ) rõ ràng để biểu diễn trực quan tỉ 
lệ tiêu xài, và có mục tổng cộng tiền đã chi tiêu.


-------------------------------LUỒNG SỰ KIỆN--------------------------------
A.CHÍNH
case 1:
	+ Mở chương trình (lần đầu chưa có dữ liệu).
	+ Điền đầy đủ dữ liệu cho 3 ô nhập.
	+ Nhấn nút 'Thêm'.
	+ Dữ liệu được cập nhật lên bảng danh sách.
	+ Biểu đồ sẽ được tính toán để vẽ lại.
	+ Kết thúc sự kiện.
case 2:
	+ Mở chương trình (đã có dữ liệu).
	+ Nạp dữ liệu từ và thể hiện lên bảng danh sách.
	+ Biểu đồ sẽ được tính toán theo dữ liệu để vẽ.
	+ Điền đầy đủ dữ liệu cho 3 ô nhập.
	+ Nhấn nút 'Thêm'.
	+ Dữ liệu được cập nhật lên bảng danh sách.
	+ Biểu đồ sẽ được tính toán để vẽ lại.
	+ Kết thúc sự kiện.
B.PHỤ
case:
	+ Mở chương trình.
	+ Điền thiếu dữ liệu cho ô 'chi phí'.
	+ Nhấn nút 'Thêm'.
	+ Hiện thông báo báo người dùng phải nhập đủ thông tin.
	+ Kết thúc sự kiện.


-----------------------------------LINK------------------------------------
-Youtube: https://youtu.be/VdSAbR7PfsE
-Bitbucket: https://thanhtri3297@bitbucket.org/thanhtri3297/1512605_quanlychitieu.git


---------------------------------GHI CHÚ-----------------------------------
1. Mục 'Loại': gồm 6 loại như yêu cầu, nếu người dùng nhập loại tùy ý (nhưng 
không được bỏ trống) sẽ được phân vào loại 'Khác'.

2. Mục 'Chi phí': có thể nhập tối đa là 18 chữ số.

3. Mục 'Loại','Nội dung': xóa khoảng trắng dư nếu có.

4. Tối ưu giao diện:
     + Tỉ lệ phần trăm lấy 2 số sau dấu phẩy. (vd: 8.99%)
     + Chuỗi tiền tổng có '.' phân cách. (vd: 1.200.000 vnđ)