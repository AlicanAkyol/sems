# Anti Sandbox and Anti Virtual Machine Tespit Aracı
Zararlı yazılım analizi gerçekleştirirken zararlı yazılımlar tarafından alınan bazı analiz engelleme yöntemleri bulunmaktadır. Bu yöntemlerden biri de sanal makine ve sandbox engellemedir.

Zararlı yazılımların sanal makine, kum havuzu ve analiz araçlarına yönelik aldıkları ya d alabilecekleri önlemlerin birçoğu bu tespit aracında mevcuttur. Tespit aracı ile zararlı yazılımların nasıl önlemler alabilecekleri ve çalışılan analiz ortamında ne tür tespit açıklıkları barındığı görülebilmektedir.

## Nasıl Çalışır?
Tespit aracı ile çalışılan ortamı test etmek için yapılacak tek şey tespit aracını ortamda çalıştırmaktır.
  - Çalışılan ortam sanal makine ise tespit aracı sanal makineye kopyalanarak/taşınarak direk çift tıklanarak çalıştırılır. Aracımız, tespit ettiği sanal makine izlerini ekrana yazmaktadır. En son "control" yazısı çıkana kadar çalışmasını sürdürür. "control" çıktısı ekranda göründüğü zaman çalışma bitmiştir. Ayrıca bulduğu sanal makine izlerinin isminde bir adet .txt dosyası oluşturmaktadır. Örnek olarak Virtualbox Bios tespiti gerçekleşti ise vboxBios.txt dosyası oluşacaktır. 
  - Test etmek istenilen ortam sandbox ise tespit aracı sandbox ortamına submit edilerek analiz işleminden geçirilir. Analiz işlemi bittikten sonra sonuçlarda dosya aktivitelerine bakılır. Eğer tespit gerçekleştir ise burada tespit edilen sanal makine veya sandbox izlerinin isminde .txt dosyaları bulunacaktır.
  
## Gerçekleştirilen Tespitler
Tespit aracının tespit ettiği araçlar ve ortamlar  aşağıdaki başlıklarda gösterilmektedir.


### VirtualBox Detection
  - Files
  - Regedit
  - Folder
  - Services
  - Mac
  - Bios
  - Window

### VMWare Detection
  - Files
  - Folder
  - Regedit
  - Services
  - Mac
  - Bios
  - Window
  - Magic
  - Memory
  - Version
  - IDTR, LDTR, TR, SMSW, I/O Port

### QEMU Detection
  - Regedit
  - Bios
  - CPU

### Cuckoo Sandbox Detection
  - Files
  - Folder
  - Port
  - Hooked Function
  - Core Number
  - Pipe
  - Modules

### Some Sandboxes Detection
Anubis , Thread Expert , Cuckoo , Sandboxie , CWSandbox
  - Computer Name
  - Core Number
  - Modules
  - Check internet
  - Disk spaces
  - Files

### Analysis Tools Detection
  - Immunity Debugger
  - Ollydbg
  - Ida Pro
  - Regshot
  - Fiddler
  - Wireshark
  - Process Monitor
  - Process Hacker
  - Process Explorer
