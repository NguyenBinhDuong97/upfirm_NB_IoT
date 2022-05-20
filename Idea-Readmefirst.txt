Ý tưởng như sau: 
Sẽ có một thiết bị riêng truyền tải dữ liệu nạp code tới 
các thiết bị khác (có kèm theo ID thiết bị). Thiết bị này nhận dữ liệu file hex
truyền từ PC tới sau đó sẽ lưu và coi như là dữ liệu cần truyền đi nhận xong sẽ
gửi các dữ liệu qua MQTT. 
*Tuy nhiên đang băn khoăn không biết nó sẽ truyền file như nào trong các 
ứng dụng đã có và mình sẽ tách các byte riêng biệt ra để truyền (hoặc hướng 
khác) sẽ phụ thuộc vào nhiều lựa chọn sau này.

/*---------------------------------------------------------------------------*/
Phase 1: Tìm hiểu về bootloader
      + Tạo ra một boot loader cơ bản với hướng dẫn sẵn có trên mạng nạp bằng 
	  công cụ và phần mềm của hãng (Folder: bootloadaer)
	  
Phase 2: Tạo một project đơn giản nhận code từ file hex qua uart để nạp chip
      _ Cần tạo một window app để truyền dữ liệu từ file hex xuống VDK
      * Trong quá trình phát triển thấy rằng các thao tác với file có sẵn sẽ
      dễ dàng hơn với các file.txt, đồng thời các file có thể copy tất cả nội 
      dung từ file hex sang một file txt khác mà nội dung không thay đổi. (Folder: Terminal_WinApp)
	  
Phase 3: Sau đó ta có thể chuyển đổi dữ liệu từ char trong text sang dữ liệu 
       số để nạp vào các địa cần thiết trong vi điều khiển. Sau khi nạp dữ liệu 
	   vào tất cả các vùng code cần nạp thì bootloader sẽ lựa chọn chạy đến vùng 
	   nào để hoạt động vùng code đó (Folder: Boot_and_WinApp)
	   
Phase 4: Sau khi nạp thì thêm các ACK, Frame, và nhiều phần khác để đảm bảo trong 
       quá trình truyền dữ liệu để không bị mất hay xáo trộn (hiện tại chưa cho thêm phần này vào 
	   tuy nhiên công ty Sao Viet cũng có phần này rồi nên sau này việc thêm phần này cũng sẽ rất dễ dàng)
	   
Phase 5: Thêm phần mqqt wirless để truyền tín hiệu qua NB_IoT
       * Hiện nay đã có file chứa các dữ liệu hex mà chỉ cần nạp dữ liệu này vào vùng flash application
	   rồi gọi ra là có thể chạy được.
	   
	   * Ý tưởng hiện nay:
	   ++ Để nạp code qua MQTT:
	      + Máy tính 
		  + HostClient(Client này sẽ đóng vai trò như một điểm phát sóng các dữ liệu được truyền 
		                từ máy tính xuống)
		  + AppClient (Client này sẽ nhận dữ liệu và ghi dữ liệu vào vùng flash application )
          + Trong App Client vùng flash được chia thành các vùng nhỏ như:
            _ Vùng boot
            _ Vùng app 1
            _ Vùng app 2
            _ Sử dụng một WORD của EEPROM để điều chỉnh vùng app nào sẽ chạy mỗi khi boot
            _ Một vùng flash riêng (1 page tạm gọi là page địa chỉ) để lưu địa chỉ WORD nạp code tiếp theo.
       ++ Quá trình thực hiện như sau:
	      + Host Client sẽ kết nối với Máy tính thông qua UART (phần cứng).
          + Máy tính mở file chứa dữ liệu WordData hex cần nạp tính tổng số word và số byte thừa cần nạp
		  + Máy tính mở file hex được copy từ hex nguồn lấy thông tin vị trí bắt đầu của application rồi
		  quy đổi ra vùng app sẽ nạp gắn vào một biến (dòng đầu tiên của file hex nguồn chứa địa chỉ 
		  bắt đầu).
          + Máy tính mở cổng COM để giao tiếp với HostClient.
		  + Máy tính cho phép nhập ID của AppClient cần nạp.
		  + Máy tính truyền ID này xuống HostClient qua cổng COM.
		  + HostClient nhận ID này và ghép vào một chuỗi sau này để subscribe cùng với topic mà mỗi AppClient 
		  đã Subcribe trong bản thân nó.
		  + HostClient nhận xong gửi lệnh thông báo đã nhận ID.
		  + Máy tính nhận đã kết nối và đợi nhận lệnh Start từ console rồi gửi xuống HostClient.
		  + HostClient nhận được start bắt đầu kết nối MQTT và gửi cho máy tính biết đã subscribe .
		  thành công.
		  + HostClient Publish một đoạn tin update start đợi AppClient phản hồi số thứ tự được lưu trong Page 
		  địa chỉ.
		  + AppClient nhận được bản tin update start thì đọc từ flash địa chỉ chứa số thứ tự của dòng code 
		  cần nạp vào app .
		  + AppClient gán giá trị của WORD trong EEPROM biểu thị app nào đang hoạt động vào một biến gọi 
		  là AppSelect.
		  + AppClient gửi giá trị AppSelect + giá trị đọc từ flash trong page địa chỉ qua Topic mà 
		  HostClient đã đăng kí.
		  + HostClient nhận được thông tin từ AppClient, phân tách gửi cho Máy tính giá trị vùng app đang 
		  chạy.
		  + Máy tính hiển thị vùng app đang chạy ở AppClient kiểm tra xem có trùng khớp với vùng app mà 
		  định nạp hay không nếu có out ra khỏi chương trình.
		  + 
		  
		  
		 
		  
		  
		  
		 
		  
