--------------------------------THÔNG TIN SINH VIÊN--------------------------------
NGUYỄN THANH TRÍ
1512605
tringuyen3297@gmail.com
Paint (with DLL)

-------------------------------CHỨC NĂNG HOÀN THÀNH--------------------------------
1. Đưa 2 hàm vào DLL và gọi lại từ ứng dụng chính (hàm CheckOrientDraw(), hàm ChangeToSpecial())
2. Vẽ được 5 loại hình cơ bản: Line, Chữ nhật, Hình vuông, Ellipse, Hình tròn.
3. Cho phép chọn loại hình, kích thước bút, màu bút cần vẽ, màu nền trong. (có checked menu để tiện quan sát)
4. Sử dụng mẫu Prototype để lưu các mẫu.
5. Không làm lưu và nạp các object. (do sử dụng Memory DC lưu các shapes)
6. Lưu và nạp hình (dạng bitmap)
7. Chức năng undo để người dùng vẽ sai 1 hình có thể quay lại trước đó 1 hình.

-----------------------------------LUỒNG SỰ KIỆN-----------------------------------
A.CHÍNH
1.
	+ Mở chương trình, chọn loại hình, bút vẽ, màu vẽ.(Không chọn sẽ xài mặc định)
	+ Vẽ.
	+ Lưu hình đã vẽ.
	+ Thoát.
	+ Kết thúc sự kiện.
2.
	+ Mở chương trình.
	+ Mở 1 file bitmap có sẵn trong máy thành công.
	+ Vẽ tiếp lên hình vừa mở.
	+ Lưu hình đã vẽ.
	+ Thoát.
	+ Kết thúc sự kiện.
B.PHỤ
	+ Mở chương trình, chọn loại hình, bút vẽ, màu vẽ.(Không chọn sẽ xài mặc định)
	+ Vẽ.
	+ Mở chức năng lưu hình nhưng cancel hay thoát khỏi window save đó.
	+ Hình vẫn còn nguyên đó.
	+ Kết thúc sự kiện.
    
-----------------------------------LINK------------------------------------
-Youtube: https://youtu.be/CRWfGSmCH1k
-Bitbucket: https://thanhtri3297@bitbucket.org/thanhtri3297/1512605_paint_dll.git
