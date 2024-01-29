#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define BUF_LEN     0x400
#define BITS_PER_TRANSFER   16

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

    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);

    bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI));

    spi_set_format(spi_default, BITS_PER_TRANSFER, 1, 1, SPI_MSB_FIRST);
    
    uint16_t out_buf[BUF_LEN];

    for(int i = 0; i < BUF_LEN; i++){
        out_buf[i] = i;
    }

    printbuf(out_buf, BUF_LEN);

    for(size_t i; ; i++){
        spi_write16_blocking(spi_default, out_buf, BUF_LEN);
    }
}