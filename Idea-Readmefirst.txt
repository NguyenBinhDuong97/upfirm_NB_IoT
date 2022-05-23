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
		  + AppClient gán giá trị của WORD trong EEPROM biểu thị app nào đang hoạt động vào một biến gọi 
		  là AppSelect và xóa flash vùng cần nạp.
		  + AppClient gửi giá trị AppSelect qua Topic mà HostClient đã đăng kí.
		  + HostClient nhận được thông tin từ AppClient, phân tách gửi cho Máy tính giá trị vùng app đang 
		  chạy.
		  + Máy tính hiển thị vùng app đang chạy ở AppClient kiểm tra xem có trùng khớp với vùng app mà 
		  định nạp hay không nếu có out ra khỏi chương trình máy tính
		  + Máy tính kiểm tra vùng app định nạp khác với vùng app đang chạy thì gửi HostClient bat dau nap
		  + HostClient nhận được bắt đầu nạp thì gắn vào một biến để hiển thị rằng đã qua lần start đầu tiên 
		  để lần sau sẽ chỉ gửi data và ack
		++ Nạp:
		  + HostClient sẽ publish thông tin "update"
		  + AppClient nhận được thông tin "update" sẽ đọc giá trị flash trong page địa chỉ
		  + AppCliet gửi giá trị trong page địa chỉ, địa chỉ cần nạp tiếp theo vào topic 
		  mà HostClient đã đăng kí
		  + HostClient nhận được thứ tự địa chỉ cần nạp rồi gửi thứ tự đó tới máy tính
		  + Máy tính nhận được thứ tự cần nạp rồi đặt con trỏ tới vị trí tương ứng trong file hex
		  + Gửi data xuống HostClient
		  + HostClient nhận data rồi gửi data qua topic
		  + AppClient nhận được data rồi phân tích chuyển đổi rồi gắn data vào vùng flash cần nạp: 
		  địa chỉ bắt đầu(tùy thuộc giá trị lưu trong eeprom) + giá trị lưu trong page địa chỉ.
		  + AppClient sau khi lưu xong giá trị cần nạp tiếp theo tăng lên 1
		  + AppClient xóa page địa chỉ. 
		  + AppClient lưu số thứ tự cần nạp tiếp theo tăng lên 1 vào page địa chỉ.
		  + AppClient gửi thông đã nhận data thành công (OK)
		  + HostClient nhận thông tin đã nạp thành công (OK)
		  + HostClient gửi thông tin đó lên Máy tính.
		  + Máy tính nhận được thông tin này rồi hiển lên console 
		  + Máy tính gửi tín hiệu update xuống hostclient
		  + Tiếp tục
		  + Đến khi hết file thì gửi end xuống HostClient
		  + HostClient gửi end 
		  + AppClient nhận end thực hiện quá trình chuẩn bị cho việc boot.
		  
		  
* Quá trình test chia ra làm hai phần.
  + Phần gửi:
    ++ HostClient
    ++ Máy tính + phần mềm MQTT explore
    ++ Dùng hai cái này để test trước
  + Phần nhận:
    ++ AppClient
	++ phần mềm MQTT explore.
	++ Dùng hai phần này để test