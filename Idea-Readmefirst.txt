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
       quá trình truyền dữ liệu để không bị mất hay xáo trộn.
Phase 5: Thêm phần mqqt wirless để truyền tín hiệu qua NB_IoT.
