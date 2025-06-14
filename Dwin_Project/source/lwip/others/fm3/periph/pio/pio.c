
//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "pio.h"
#include <board.h>

//------------------------------------------------------------------------------
//         Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Configures one or more pin(s) of a PIO controller as being controlled by
/// peripheral A. Optionally, the corresponding internal pull-up(s) can be
/// enabled.
/// \param pio  Pointer to a PIO controller.
/// \param mask  Bitmask of one or more pin(s) to configure.
/// \param enablePullUp  Indicates if the pin(s) internal pull-up shall be
///                      configured.
//------------------------------------------------------------------------------
/*static void PIO_SetPeripheralA(
    AT91S_PIO *pio,
    unsigned int mask,
    unsigned char enablePullUp)
{
    // Disable interrupts on the pin(s)
    pio->PIO_IDR = mask;

    // Enable the pull-up(s) if necessary
    if (enablePullUp) {

        pio->PIO_PPUER = mask;
    }
    else {

        pio->PIO_PPUDR = mask;
    }

    // Configure pin
    pio->PIO_ASR = mask;
    pio->PIO_PDR = mask;
}*/

//------------------------------------------------------------------------------
/// Configures one or more pin(s) of a PIO controller as being controlled by
/// peripheral B. Optionally, the corresponding internal pull-up(s) can be
/// enabled.
/// \param pio  Pointer to a PIO controller.
/// \param mask  Bitmask of one or more pin(s) to configure.
/// \param enablePullUp  Indicates if the pin(s) internal pull-up shall be
///                      configured.
//------------------------------------------------------------------------------
/*static void PIO_SetPeripheralB(
    AT91S_PIO *pio,
    unsigned int mask,
    unsigned char enablePullUp)
{
    // Disable interrupts on the pin(s)
    pio->PIO_IDR = mask;

    // Enable the pull-up(s) if necessary
    if (enablePullUp) {

        pio->PIO_PPUER = mask;
    }
    else {

        pio->PIO_PPUDR = mask;
    }

    // Configure pin
    pio->PIO_BSR = mask;
    pio->PIO_PDR = mask;
}*/

//------------------------------------------------------------------------------
/// Configures one or more pin(s) or a PIO controller as inputs. Optionally,
/// the corresponding internal pull-up(s) and glitch filter(s) can be
/// enabled.
/// \param pio  Pointer to a PIO controller.
/// \param mask  Bitmask indicating which pin(s) to configure as input(s).
/// \param enablePullUp  Indicates if the internal pull-up(s) must be enabled.
/// \param enableFilter  Indicates if the glitch filter(s) must be enabled.
//------------------------------------------------------------------------------
/*static void PIO_SetInput(
    AT91S_PIO *pio,
    unsigned int mask,
    unsigned char enablePullUp,
    unsigned char enableFilter)
{
    // Disable interrupts
    pio->PIO_IDR = mask;

    // Enable pull-up(s) if necessary
    if (enablePullUp) {

        pio->PIO_PPUER = mask;
    }
    else {

        pio->PIO_PPUDR = mask;
    }

    // Enable filter(s) if necessary
    if (enableFilter) {

        pio->PIO_IFER = mask;
    }
    else {

        pio->PIO_IFDR = mask;
    }

    // Configure pin as input
    pio->PIO_ODR = mask;
    pio->PIO_PER = mask;
}*/

//------------------------------------------------------------------------------
/// Configures one or more pin(s) of a PIO controller as outputs, with the
/// given default value. Optionally, the multi-drive feature can be enabled
/// on the pin(s).
/// \param pio  Pointer to a PIO controller.
/// \param mask  Bitmask indicating which pin(s) to configure.
/// \param defaultValue  Default level on the pin(s).
/// \param enableMultiDrive  Indicates if the pin(s) shall be configured as
///                          open-drain.
/// \param enablePullUp  Indicates if the pin shall have its pull-up activated.
//------------------------------------------------------------------------------
/*static void PIO_SetOutput(
    AT91S_PIO *pio,
    unsigned int mask,
    unsigned char defaultValue,
    unsigned char enableMultiDrive,
    unsigned char enablePullUp)
{
    // Disable interrupts
    pio->PIO_IDR = mask;

    // Enable pull-up(s) if necessary
    if (enablePullUp) {

        pio->PIO_PPUER = mask;
    }
    else {

        pio->PIO_PPUDR = mask;
    }

    // Enable multi-drive if necessary
    if (enableMultiDrive) {

        pio->PIO_MDER = mask;
    }
    else {

        pio->PIO_MDDR = mask;
    }

    // Set default value
    if (defaultValue) {

        pio->PIO_SODR = mask;
    }
    else {

        pio->PIO_CODR = mask;
    }

    // Configure pin(s) as output(s)
    pio->PIO_OER = mask;
    pio->PIO_PER = mask;
}*/

//------------------------------------------------------------------------------
//         Global Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Configures a list of Pin instances, each of which can either hold a single
/// pin or a group of pins, depending on the mask value; all pins are configured
/// by this function. The size of the array must also be provided and is easily
/// computed using PIO_LISTSIZE whenever its length is not known in advance.
/// \param list  Pointer to a list of Pin instances.
/// \param size  Size of the Pin list (calculated using PIO_LISTSIZE).
/// \return 1 if the pins have been configured properly; otherwise 0.
//------------------------------------------------------------------------------
unsigned char PIO_Configure(const Pin *list, unsigned int size)
{
    // Configure pins
    /*while (size > 0) {

        switch (list->type) {

            case PIO_PERIPH_A:
                PIO_SetPeripheralA(list->pio,
                                   list->mask,
                                   (list->attribute & PIO_PULLUP) ? 1 : 0);
                break;

            case PIO_PERIPH_B:
                PIO_SetPeripheralB(list->pio,
                                   list->mask,
                                   (list->attribute & PIO_PULLUP) ? 1 : 0);
                break;

            case PIO_INPUT:
                AT91C_BASE_PMC->PMC_PCER = 1 << list->id;
                PIO_SetInput(list->pio,
                             list->mask,
                             (list->attribute & PIO_PULLUP) ? 1 : 0,
                             (list->attribute & PIO_DEGLITCH)? 1 : 0);
                break;

            case PIO_OUTPUT_0:
            case PIO_OUTPUT_1:
                PIO_SetOutput(list->pio,
                              list->mask,
                              (list->type == PIO_OUTPUT_1),
                              (list->attribute & PIO_OPENDRAIN) ? 1 : 0,
                              (list->attribute & PIO_PULLUP) ? 1 : 0);
                break;

            default: return 0;
        }

        list++;
        size--;
    }

    return 1;*/
}

//------------------------------------------------------------------------------
/// Sets a high output level on all the PIOs defined in the given Pin instance.
/// This has no immediate effects on PIOs that are not output, but the PIO
/// controller will memorize the value they are changed to outputs.
/// \param pin  Pointer to a Pin instance describing one or more pins.
//------------------------------------------------------------------------------
void PIO_Set(const Pin *pin)
{
    //pin->pio->PIO_SODR = pin->mask;
}

//------------------------------------------------------------------------------
/// Sets a low output level on all the PIOs defined in the given Pin instance.
/// This has no immediate effects on PIOs that are not output, but the PIO
/// controller will memorize the value they are changed to outputs.
/// \param pin  Pointer to a Pin instance describing one or more pins.
//------------------------------------------------------------------------------
void PIO_Clear(const Pin *pin)
{
   // pin->pio->PIO_CODR = pin->mask;
}

//------------------------------------------------------------------------------
/// Returns 1 if one or more PIO of the given Pin instance currently have a high
/// level; otherwise returns 0. This method returns the actual value that is
/// being read on the pin. To return the supposed output value of a pin, use
/// PIO_GetOutputDataStatus() instead.
/// \param pin  Pointer to a Pin instance describing one or more pins.
/// \return 1 if the Pin instance contains at least one PIO that currently has
/// a high level; otherwise 0.
//------------------------------------------------------------------------------
unsigned char PIO_Get(const Pin *pin)
{
   /* unsigned int reg;
    if ((pin->type == PIO_OUTPUT_0) || (pin->type == PIO_OUTPUT_1)) {

        reg = pin->pio->PIO_ODSR;
    }
    else {

        reg = pin->pio->PIO_PDSR;
    }

    if ((reg & pin->mask) == 0) {

        return 0;
    }
    else {

        return 1;
    }*/
}


//------------------------------------------------------------------------------
/// Returns 1 if one or more PIO of the given Pin are configured to output a
/// high level (even if they are not output).
/// To get the actual value of the pin, use PIO_Get() instead.
/// \param pin  Pointer to a Pin instance describing one or more pins.
/// \return 1 if the Pin instance contains at least one PIO that is configured
/// to output a high level; otherwise 0.
//------------------------------------------------------------------------------
unsigned char PIO_GetOutputDataStatus(const Pin *pin)
{
    /*if ((pin->pio->PIO_ODSR & pin->mask) == 0) {

        return 0;
    }
    else {

        return 1;
    }*/
}

//------------------------------------------------------------------------------
/// Returns the value of ISR for the PIO controller of the pin.
/// Reading this register acknoledges all the ITs.
/// \param pin  Pointer to a Pin instance describing one or more pins.
//------------------------------------------------------------------------------
unsigned int PIO_GetISR(const Pin *pin)
{
    //return (pin->pio->PIO_ISR);
}

