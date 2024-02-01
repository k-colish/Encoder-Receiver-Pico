#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define BUF_LEN     0x100
#define BITS_PER_TRANSFER   16
#define PICO_SPI_SCK_PIN 19
#define PICO_SPI_RX_PIN 16
#define PICO_SPI_TX_PIN 20

void printbuf(uint16_t buf[], size_t len){
    int i;
    for (i = 0; i < len; i++){
        if(i % 16 == 15)
            printf("%03x\n", buf[i]);
        else   
            printf("%03x ", buf[i]);
    }

    if (i % 16){
        putchar('\n');
    }
}

int main(){
    
    stdio_init_all();

    spi_init(spi_default, 1000 * 1000);

    gpio_set_function(PICO_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);

    bi_decl(bi_4pins_with_func(PICO_SPI_RX_PIN, PICO_SPI_TX_PIN, PICO_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI));

    spi_set_format(spi_default, BITS_PER_TRANSFER, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    
    uint16_t out_buf[BUF_LEN];
    uint16_t in_buf[BUF_LEN];

    for(int i = 0; i < BUF_LEN; i++){
        out_buf[i] = i;
    }

    printbuf(out_buf, BUF_LEN);

    while(true){
        gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);
        
        if(spi_is_writable(spi_default)){
        spi_write16_read16_blocking(spi_default, out_buf, in_buf, BUF_LEN);
        printbuf(in_buf, BUF_LEN);
        sleep_ms(1000);
        } 

        else printf("SPI not available\n");  

        gpio_put(PICO_DEFAULT_SPI_CSN_PIN, 0);
    }
}