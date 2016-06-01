# Anti Sandbox and Anti Virtual Machine Tespit Aracı
( Sha1 (sems.exe) : 9ED1C415699FC4CD0C74EA9FC1FE97D7AF847CA0)

Zararlı yazılım analizi gerçekleştirirken zararlı yazılımlar tarafından alınan bazı analiz engelleme yöntemleri ile karşılaşılmaktadır. Bu yöntemlerden biri de analiz ortamı kontrolüdür. Analiz ortamı kontrolünde sanal makine, sandbox ve analiz araçlarının varlığı kontrol edilmektedir.

Zararlı yazılımların sanal makine, kum havuzu ve analiz araçlarına yönelik aldıkları ya da alabilecekleri önlemlerin birçoğu bu tespit aracında mevcuttur. Tespit aracı ile zararlı yazılımların nasıl önlemler alabilecekleri ve çalışılan analiz ortamında ne tür tespit açıklıkları barındığı görülebilmektedir.

## Nasıl Çalışır?
Tespit aracı ile çalışılan ortamı test etmek için yapılacak tek şey tespit aracını ortamda çalıştırmaktır.
  - Çalışılan ortam sanal makine ise tespit aracı sanal makineye kopyalanarak/taşınarak direk çift tıklanarak çalıştırılır. Aracımız, tespit ettiği sanal makine izlerini ekrana yazmaktadır. En son "control" yazısı çıkana kadar çalışmasını sürdürür. "control" çıktısı ekranda göründüğü zaman çalışma bitmiştir. Ayrıca bulduğu sanal makine izlerinin isminde bir adet .txt dosyası oluşturmaktadır. Örnek olarak Virtualbox Bios tespiti gerçekleşti ise vboxBios.txt dosyası oluşacaktır. 
  
  - Test etmek istenilen ortam sandbox ise tespit aracı sandbox ortamına submit edilerek analiz işleminden geçirilir. Analiz işlemi bittikten sonra sonuçlarda dosya aktivitelerine bakılır. Eğer tespit gerçekleştir ise burada tespit edilen sanal makine veya sandbox izlerinin isminde .txt dosyaları bulunacaktır.
  
## Gerçekleştirilen Tespitlerden Örnekler
Tespit aracını Cuckoo Sandbox' a gönderdiğimizde alınan sonuç aşağıda gösterilmektedir; (7 Nisan 2016) 
(Link : https://malwr.com/analysis/OWZmMWYzNmJkNDM3NGExMWFjODY2MGE2OWZmZjQzZjE/) 

![alt tag](https://github.com/AlicanAkyol/sems/blob/master/cuckooResult.png)

Tespit aracı VMWare sanallaştırma ortamında çalıştırıldığında alınan sonuç;
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/vmware_normal.png)

Tespit aracı Virtualbox sanallaştırma ortamında çalıştırıldığında alınan sonuç;
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/Sanalla%C5%9Ft%C4%B1rmaOrtam%C4%B1TespitArac%C4%B1-2.png)

Sems, Cuckoo'da çalıştırıldığı zaman alınan sonuç:
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/cuckoo_tespitler.png)

Cuckoo full rapor : https://malwr.com/analysis/OTJmMDlhOWViMjlhNGY1MDgzNmM5ZDMzZGZlZjI2ZDg/

Sems, ThreatExpert'te çalıştırıldığı zaman alınan sonuç:
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/threatexpert_tespitler.png)

Threatexpert full rapor: http://www.threatexpert.com/report.aspx?md5=b75f84ea8a08eade90e7afd499cd14e1

Sems, Comodo'da çalıştırıldığı zaman alınan sonuç:
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/comodo_tespitler.png)

Comodo full rapor : http://camas.comodo.com/cgi-bin/submit?file=8a7962a180d09fe3274c09abe4eb9182b500360cb72ef2f1070226db4c01e699

Sems, Payload Security'de çalıştırıldığı zaman alınan sonuç:
![alt tag](https://github.com/AlicanAkyol/sems/blob/master/payload_tespitler.png)

Payload Security full rapor: https://www.hybrid-analysis.com/sample/3a5481d105673bf20256512c9a32b60e946a240c1793e2603c226c788f234055?environmentId=1

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
