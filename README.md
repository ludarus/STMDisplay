# Specifications
- Demo image = 153600B (unoptimized size)
- F091RC SRAM = 32KB
- 

# Image Display Time Optimization
**Initial Implementation**
- time = 517 ms

**Switch from blocking HAL_SPI_Transmit to non-blocking HAL_SPI_Transmit_DMA**
- time = 520 ms
- better power efficiency and CPU usage due to Direct Memory Access

**Decrease SPI clock prescaler from 16 to 4**
- time = 138 ms
- increased data transfer rate

**Rewrote chunking algorithm**
 - time = 128 ms

**Added dual buffer and optimized chunk size**
- allows transfer of next chunks into MCU RAM while current transfer is happening 
- chunk = 256B
    - time = 148 ms
- chunk = 512B
    - time = 138 ms
- chunk = 1024B
    - time = 133 ms
- chunk = 2048B
    - time = 130 ms
- chunk = 4096B
    - time = 121 ms
- chunk = 8192B
    - time = 102 ms
- chunk = 16384B
    - too big to fit into RAM



# Proof of Image Display
- 
![demo of working image display](/demo.jpg)
