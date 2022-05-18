Sau khi thực hiện những ý tưởng trước (chưa gắn phần truyên thông wireless vào)
phần này sẽ bổ sung cho phần wiresless này thêm được thực hiện với các bước như
sau:
+ Xem lại phần Event Drivent và áp dụng code theo cái thư viện được build 
từ trước.
+ Build code với trường hợp giả định không có lỗi nào xảy ra trong quá trình 
upfirm.
+ Build code với trường hợp có lỗi trong quá trình upfirm:
  ++ Đang truyền bị ngắt kết nối
  ++ Đang truyền thiết bị đưa vào ngủ
    (Hiện nay đang có ý tưởng giải quyết đó là trong khi upfirm sẽ tạm thời tắt 
	chế độ ngủ của thiết bị đi và sẽ quét một biến lưu địa chỉ vừa nạp xong mỗi
	1 phút nếu địa chỉ không thay đổi thì lưu địa chỉ đang nạp vào một ô flash
	rồi truyền lại khi bắt đầu có tín hiệu nạp từ thiết bị nạp).
	