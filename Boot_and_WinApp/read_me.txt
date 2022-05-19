# ver1 để tạo project với truyền nhận cơ bản 
# ver2 để tạo project có một số win_app:
  + Tách WinApp ra làm hai phần mềm:
    ++ Một phần mềm là để tách dữ liệu từ file hex source 
	đến một file chỉ chứa các Word của file hex (mỗi word một dòng và 
	convert sang little endian).
    ++ Một phần mềm là để lựa chọn số Word trong file được tách từ ứng dụng 
	trên truyền xuống thiết bị nạp rồi truyền qua MQTT thiết bị nhận sẽ nhận
    để ghi vào flash trong thiết bị nhận.
    ++ Phát hiện ra lỗi đó là không phải lúc nào chương trình nạp số word
	cũng là một số nguyên chẵn cũng có thể sẽ thêm một số byte lẻ lên thay đổi
	cách truyền.
	
	