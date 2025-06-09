

#ifndef PIO_H
#define PIO_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>

//------------------------------------------------------------------------------
//         Global Definitions
//------------------------------------------------------------------------------

/// The pin is controlled by the associated signal of peripheral A.
#define PIO_PERIPH_A                0
/// The pin is controlled by the associated signal of peripheral B.
#define PIO_PERIPH_B                1
/// The pin is an input.
#define PIO_INPUT                   2
/// The pin is an output and has a default level of 0.
#define PIO_OUTPUT_0                3
/// The pin is an output and has a default level of 1.
#define PIO_OUTPUT_1                4

/// Default pin configuration (no attribute).
#define PIO_DEFAULT                 (0 << 0)
/// The internal pin pull-up is active.
#define PIO_PULLUP                  (1 << 0)
/// The internal glitch filter is active.
#define PIO_DEGLITCH                (1 << 1)
/// The pin is open-drain.
#define PIO_OPENDRAIN               (1 << 2)

//------------------------------------------------------------------------------
//         Global Macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Calculates the size of an array of Pin instances. The array must be defined
/// locally (i.e. not a pointer), otherwise the computation will not be correct.
/// \param pPins  Local array of Pin instances.
/// \return Number of elements in array.
//------------------------------------------------------------------------------
#define PIO_LISTSIZE(pPins)    (sizeof(pPins) / sizeof(Pin))

//------------------------------------------------------------------------------
//         Global Types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Describes the type and attribute of one PIO pin or a group of similar pins.
/// The #type# field can have the following values:
///    - PIO_PERIPH_A
///    - PIO_PERIPH_B
///    - PIO_OUTPUT_0
///    - PIO_OUTPUT_1
///    - PIO_INPUT
///
/// The #attribute# field is a bitmask that can either be set to PIO_DEFAULt,
/// or combine (using bitwise OR '|') any number of the following constants:
///    - PIO_PULLUP
///    - PIO_DEGLITCH
///    - PIO_OPENDRAIN
//------------------------------------------------------------------------------
typedef struct {

    /// Bitmask indicating which pin(s) to configure.
    unsigned int mask;
    uint32_t iopin;
    /// Pointer to the PIO controller which has the pin(s).
    uint8_t    pio;
    /// Peripheral ID of the PIO controller which has the pin(s).
    unsigned char id;
    /// Pin type.
    unsigned char type;
    /// Pin attribute.
    unsigned char attribute;

} Pin;

//------------------------------------------------------------------------------
//         Global Functions
//------------------------------------------------------------------------------

extern unsigned char PIO_Configure(const Pin *list, unsigned int size);

extern void PIO_Set(const Pin *pin);

extern void PIO_Clear(const Pin *pin);

extern unsigned char PIO_Get(const Pin *pin);

extern unsigned int PIO_GetISR(const Pin *pin);

extern unsigned char PIO_GetOutputDataStatus(const Pin *pin);

#endif //#ifndef PIO_H

