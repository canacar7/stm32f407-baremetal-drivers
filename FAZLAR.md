# Hafta 1 Ödevi — Faz Takibi

**Ödev:** Yük Altında Buton Yanıt Süresi
**Kart:** STM32F407VG (DISC1) · bare-metal, kendi register sürücülerim
**Son teslim:** 27 Eylül 2026, Pazar 11:00 (TR saati)

Her fazın sonunda **doğrulanabilir** bir çıktı var. Bir faz "bitti" sayılmadan
sonrakine geçme — asıl vakit kaybı iki katmanı aynı anda debug etmekte.

---

## Faz 1 — USART polling (RTOS yok)

Hedef: hat çalışıyor mu, onu bir daha hiç sormamak.

- [ ] RCC: USART ve GPIO clock enable
- [ ] GPIO alternate function ayarı (TX/RX pinleri)
- [ ] `BRR` hesabı — **dikkat:** `SystemInit` boş, çip HSI 16 MHz'de
- [ ] `CR1`: TE + UE
- [ ] `USART_TransmitData()` polling ile: `while(!(SR & TXE)); DR = c;`
- [ ] Tek karakter → sonra string

**Bitti sayılır:** minicom / PC arayüzü 115200 8N1'de yazıyı doğru görüyor.

> **Donanım notu:** F407 DISC1'de ST-Link'in sanal COM portu MCU'ya bağlı değil.
> Büyük ihtimalle USB-TTL çevirici (CH340 / CP2102 / FT232) lazım —
> TX / RX / GND üç kablo.

---

## Faz 2 — USART TX kesmesi

- [ ] `TXEIE` set, NVIC'te USART kesmesini aç
- [ ] `USARTx_IRQHandler`: index sayacı ile bayt bayt besleme
- [ ] Baytlar bitince `TXEIE → 0`, `TCIE → 1`
- [ ] TC kesmesinde bitir, bayrağı temizle
- [ ] `volatile` paylaşılan durum: `tx_buf`, `tx_idx`, `tx_len`

**Bitti sayılır:** `main()` içindeki `while(1)` bomboşken 64 baytlık mesaj çıkıyor.

> **TXE ≠ TC.** TXE "veri yazmacı boşaldı, sıradakini ver" demek — bayt henüz
> hatta çıkmadı. TC "kaydırma yazmacı da boşaldı, son bit hattan çıktı" demek.
> Ödevde t₄ **TC**'de kaydedilecek. Bu ayrım ölçümün doğruluğunun temeli.

---

## Faz 3 — Mikrosaniye zaman kaynağı

Bu ölçüm aletin. Beş zaman noktası da bunu okuyacak.

- [ ] TIM2 (32-bit), prescaler ile 1 MHz
- [ ] `ARR = 0xFFFFFFFF`, serbest sayan, taşma kesmesi yok
- [ ] `timer_us()` → `TIM2->CNT` (tek okuma, kilitlemeye gerek yok)

**Bitti sayılır:** 1 saniye bekle, iki okumanın farkını bas → ~1000000 çıkmalı.

> SysTick kullanma: 24-bit, geri sayıyor ve Faz 5'te FreeRTOS onu sahiplenecek.
> Çözünürlük 1 µs olmak zorunda — ölçeceğin `t₂−t₁` aralığı ~0,3 ms mertebesinde,
> 1 ms'lik tick ile bu aralık ölçülemez.

---

## Faz 4 — Buton EXTI

- [ ] EXTI hattı + NVIC (PA0 zaten çalışıyor, üstüne kurulacak)
- [ ] ISR girişinde `t₀ = timer_us()`
- [ ] 30 ms tekrar-kenar filtresi — **ilk basış filtreden atılmamalı**, ayrı ilk-olay durumu
- [ ] Olay kimliği sayacı (`next_id()`)
- [ ] Sadece basış kenarı

**Bitti sayılır:** butona basınca `BTN,17,t0=1234567` satırı UART'tan çıkıyor.
TX bu aşamada hâlâ polling olabilir.

> Bu fazın sonunda bare-metal zincirin tamamı çalışıyor. RTOS'a hiç dokunmadın.

---

## Faz 5 — FreeRTOS'u ayrı projede öğren

**Boş bir projede** yap. Faz 1–4'teki çalışan kodu kurcalama.

- [ ] Kernel + ARM_CM4F portu + heap_4 ekle, `FreeRTOSConfig.h` yaz
- [ ] `SVC_Handler` / `PendSV_Handler` / `SysTick_Handler` bağlantısı
- [ ] İki görev, farklı öncelik, LED toggle → preemption'ı gözle
- [ ] `vTaskDelay` ile `xTaskDelayUntil` farkı → periyot kayması
- [ ] `xQueueSend` / `xQueueReceive` ile görevler arası mesaj
- [ ] **ISR'den `xSemaphoreGiveFromISR` ile görev uyandırma**

**Bitti sayılır:** son madde çalışıyor.

> Son madde birebir `UartTxTask` deseni. Onu anladığın an Faz 6 mekanik bir işe döner.

---

## Faz 6 — Birleştirme

Faz 2–4 sürücülerini **olduğu gibi** al, üstüne RTOS katmanını koy.

- [ ] 3 görev: `TelemetryTask` (3) · `ButtonTask` (2) · `UartTxTask` (1)
- [ ] 2 kuyruk: `buttonQ` (8 olay) · `txQ` (16 mesaj, FIFO)
- [ ] 1 semafor: TX-complete
- [ ] `configMAX_PRIORITIES ≥ 4`, preemptive
- [ ] NVIC priority grouping: tüm bitler preemption
- [ ] **EXTI ve USART önceliği** `configMAX_SYSCALL_INTERRUPT_PRIORITY`'den
      sayısal olarak büyük/eşit (mantıksal olarak daha düşük)
- [ ] `configASSERT` tanımlı
- [ ] 64+ olaylık RAM kayıt havuzu + taşma sayacı
- [ ] Sayaçlar: kabul, buton drop, tx drop, UART hata, timeout

**Bitti sayılır:** S0 senaryosu koşuyor, CSV dışa aktarılıyor.

> Görev/kesme iş bölümü: telemetri ve buton görevleri UART'a **dokunmaz**,
> sadece `txQ`'ya mesaj bırakır. `UartTxTask` tek sahiptir.

---

## Faz 7 — Ölçüm ve rapor

Altı senaryo, her birinde **en az 30 kabul edilen basış** (toplam ≥180):

| ID | Telemetri | Ek CPU işi |
|----|-----------|------------|
| S0 | kapalı (görev **bloklanmalı**, boş dönmemeli) | yok |
| S1 | 10 Hz · 100 ms | yok |
| S2 | 50 Hz · 20 ms | yok |
| S3 | 100 Hz · 10 ms | yok |
| S4 | 100 Hz · 10 ms | ~2 ms |
| S5 | 100 Hz · 10 ms | ~5 ms |

Her senaryoda aynı sıra:

- [ ] Senaryoyu seç, önceki TX'i bitir, kayıtları/sayaçları sıfırla
- [ ] Frekans + 64 bayt mesaj boyu + CPU işini doğrula
- [ ] 5 saniye ısınma
- [ ] ≥30 basış, aralarında ≥0,5 s, basış zamanlarını değiştir
- [ ] Telemetriyi durdur, TX'i bitir (veya timeout kaydet), sonra dışa aktar

Rapor:

- [ ] Ham CSV: `scenario,event_id,t0_us,t1_us,t2_us,t3_us,t4_us,status`
- [ ] Her senaryo: başarılı sayı, R min/ort/**gözlenen maks**, 20 ms aşan sayı
- [ ] Drop / TX hata / timeout / kayıt kaybı sayıları ayrı ayrı
- [ ] Grafik 1: olay no → R, üzerinde 20 ms deadline çizgisi
- [ ] Grafik 2: senaryo → aşama süreleri, yığılmış sütun
- [ ] Grafik üretme kodu da depoda
- [ ] MCU saati, tick hızı, timer ve derleme ayarları raporda

> Eksik zamanı 0 yapma, boş bırak. `uint32` farklarını mod 2³² hesapla.
> Gözlenen maksimum kanıtlanmış worst-case değildir — öyle yazma.

---

## Ölçüm noktaları (ezberlenecek)

| Nokta | Nerede |
|-------|--------|
| t₀ | Buton ISR girişi, filtrenin kabul ettiği kenar |
| t₁ | `ButtonTask` olayı aldıktan hemen sonra |
| t₂ | Yanıt için `xQueueSend` çağrısından hemen önce |
| t₃ | UART başlatma çağrısından hemen önce (`UartTxTask` içinde) |
| t₄ | UART **TC** kesmesinde |

`R = t₄ − t₀` · deadline 20 ms · deney timeout 1 s

Beşi de **aynı** kart saatinden. PC saatiyle MCU saatini birbirinden çıkarma.

---

## Teslim yapısı

```
freertos-bootcamp/
└── hafta-01/
    ├── README.md
    ├── firmware/
    ├── interface/
    ├── measurements/
    │   ├── S0.csv … S5.csv
    │   └── summary.csv
    └── analysis/
        ├── report.md
        └── plots/
```
