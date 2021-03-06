/// \file   spibus.h
/// \author Elia Carrara
/// \date   April, 2016
/// \brief  Contains the definition of the spi-bus class.
/// \details This class abstracts almost all functionallities of the
/// spi-bus. It makes direct calls to the linux spi driver. Checkout
/// <linux/spi/spidev.h> for further information.

#ifndef SPIBUS_H
#define SPIBUS_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <bus.h>
#include <types.h>

namespace Communication{

typedef Word spi_mode;
inline const char * GetSpiModeDescription(spi_mode ucMode){
#define TXT_CPHA_0 "Sampling occures on the leading phase of the clock signal."
#define TXT_CPHA_1 "Sampling occures on the trailing phase of the clock signal."
#define TXT_CPOL_0 "Clock signal starts low, so the first (leading) edge is rising, and the second (trailing) edge is falling. "
#define TXT_CPOL_1 "Clock signal starts high, so the first (leading) edge is falling, and the second (trailing) edge is rising. "

#define TXT_SPI_MODE_0  TXT_CPOL_0 TXT_CPHA_0
#define TXT_SPI_MODE_1  TXT_CPOL_0 TXT_CPHA_1
#define TXT_SPI_MODE_2  TXT_CPOL_1 TXT_CPHA_0
#define TXT_SPI_MODE_3  TXT_CPOL_1 TXT_CPHA_1

    switch (ucMode) {
    case 0:
        return TXT_SPI_MODE_0;
    case 1:
        return TXT_SPI_MODE_1;
    case 2:
        return TXT_SPI_MODE_2;
    case 3:
        return TXT_SPI_MODE_3;
    default:
        return "Invalid SPI mode";
    }
}


class SpiBus : public Bus{
    int m_nFd;
    const char * m_strDevFile = NULL;
    spi_ioc_transfer m_sSpiMsg;
public:
    SpiBus(const char * strFile);
    ~SpiBus();

    bool IsOpen();
    bool Open();
    bool Close();

    bool Transact(Word * pcReadBuffer, const Word * pcWriteBuffer, Size nLength);

    bool GetMode(spi_mode & ucMode);
    bool GetBitsPerWord(BitsPerWord & ucBPW);
    bool GetSpeed(Hertz & unSpeed);
    bool GetDelay(MicroSeconds & unDelay);
    bool GetEndianness(Endianess & eEndianess);

    bool SetMode(spi_mode & ucMode);
    bool SetBitsPerWord(BitsPerWord & ucBPW);
    bool SetSpeed(Hertz & unSpeed);
    bool SetDelay(MicroSeconds & unDelay);
    bool SetEndianness(Endianess & eEndianess);
};

}

#endif
