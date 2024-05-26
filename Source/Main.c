#include <Anvie/Common.h>
#include <Anvie/Types.h>

/* crossfile */
#include <Anvie/CrossFile/Otf/Otf.h>

// #define TTF_FONT_PATH "../Assets/Files/FontFiles/monospace-font/MonospaceBold-zmP0.ttf"
// #define TTF_FONT_PATH "../Assets/Files/FontFiles/monospace-font/MonospaceOblique-5meB.ttf"
#define TTF_FONT_PATH "/home/misra/Documents/Olivia-Regular.otf"

static const CString font_files2[] = {
    "/home/misra/Desktop/samples/./Eiffel 65/Eiffel 65 logos/Gadget.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Eiffel 65 logos/Decoder.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Eiffel 65 logos/Alien League.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Eiffel 65 logos/Mistral.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Eiffel 65 logos/Crillee.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Eiffel 65 logos/Interstate Bold.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Eiffel 65 logos/Orator Std.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Eiffel 65 logos/Golden Girdle (Normal).ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Eiffel 65 logos/Monotype.com (Regular).ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Italian Album/Quelli Che Non Hanno Età/New "
    "Brilliant.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Italian Album/Quelli Che Non Hanno Età/7days.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Italian Album/Cosa Resterà/Strokey Bacon.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Italian Album/Una Notte E Forse Mai "
    "Più/Haettenschweiler.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Italian Album/Viaggia Insieme A Me/Especial Kay.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Italian Album/Viaggia Insieme A Me/Rambo Killer.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Italian Album/Voglia di Dance Allnight/Coolvetica.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Too Much of Heaven/Nasalization.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Too Much of Heaven/Envision Roman.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Too Much of Heaven/Abadi Mt Condensed "
    "(Light).ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/ITC Veljovic Black Italic.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Times (Bold Italic).ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Impact LT.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Move Your Body/AG Book Stencil.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Move Your Body/Continuum (Light).ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Move Your Body/Geometr212 BkCn BT (Book).ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Move Your Body/Continuum (Medium).ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Bankgothic.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Blue (Da Ba Dee)/Base 02.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Blue (Da Ba Dee)/CarbonType.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Blue (Da Ba Dee)/Hobo D.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Blue (Da Ba Dee)/Capture It.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Blue (Da Ba Dee)/Droid.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Blue (Da Ba Dee)/3000.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Blue (Da Ba Dee)/Meta Bold.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Blue (Da Ba Dee)/Airlock.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Blue (Da Ba Dee)/Uptown.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/My Consolle/Eurostile (Bold Extended).ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Europop/Silicon World/Stop (Regular).ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Contact!/Lucky (In My Life)/Badloc ICG.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Contact!/Axaxax.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Contact!/One Goal/Supreme.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Contact!/One Goal/Detonate BRK.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Contact!/80's Stars/Babylon5.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Contact!/Sleep Talk.ttf",
    "/home/misra/Desktop/samples/./Eiffel 65/Największe Przeboje/Futura (Extra Bold Condensed).ttf",
    "/home/misra/Desktop/samples/./Gabry Ponte/Ambulance Shotgun.ttf",
    "/home/misra/Desktop/samples/./Gabry Ponte/Vivaldi.ttf",
    "/home/misra/Desktop/samples/./Gabry Ponte/Floralies.ttf",
    "/home/misra/Desktop/samples/./Gabry Ponte/Establo.ttf",
    "/home/misra/Desktop/samples/./Gabry Ponte/Gunplay.ttf",
    "/home/misra/Desktop/samples/./Gabry Ponte/Wierdo.ttf",
    "/home/misra/Desktop/samples/./Gabry Ponte/Love Letter TW.ttf",
    "/home/misra/Desktop/samples/./Gabry Ponte/Eurostile Ste DBla [Extended].ttf",
    "/home/misra/Desktop/samples/./Gabry Ponte/Nightmare 5.ttf",
    "/home/misra/Desktop/samples/./Bloom 06/BN 3rd Place.ttf",
    "/home/misra/Desktop/samples/./Bloom 06/Birth of a Hero.ttf",
    "/home/misra/Desktop/samples/./Bloom 06/Defused.ttf",
    "/home/misra/Desktop/samples/./Bloom 06/Ruach LET Plain.ttf",
    "/home/misra/Desktop/samples/./Bloom 06/dearJoe 1 [demo version].ttf",
    "/home/misra/Desktop/samples/./Bloom 06/CGF Locust Resistance.ttf",
    "/home/misra/Desktop/samples/./Bliss Team/Delta Hey Max Nine.ttf",
    "/home/misra/Desktop/samples/./Zorotl/Corpus Gothic Alternative (Italic).ttf",
    "/home/misra/Desktop/samples/./Maury Lobina/Intro.ttf",
    "/home/misra/Desktop/samples/./Jeffrey Jey/ARB-218.ttf",
    "/home/misra/Desktop/samples/./Jeffrey Jey/Myriad Pro (Light).ttf",
    "/home/misra/Desktop/samples/./Bliss Corporation/Earth.ttf",
};

Int32 main() {
    XfOtfFile otf_file;

    for (Size s = 0; s < ARRAY_SIZE (font_files2); s++) {
        GOTO_HANDLER_IF (
            !xf_otf_file_open (&otf_file, TTF_FONT_PATH),
            SKIP_FONT,
            "Failed to open OTF file \"%s\" (%zu)\n\n",
            font_files2[s],
            s
        );

        if (s == 0) {
            xf_otf_file_pprint (&otf_file);
        }

        for (Size m = 0; m < otf_file.cmap.num_tables; m++) {
            if (otf_file.cmap.encoding_records[m].sub_table.format == 2) {
                PRINT_ERR ("%s\n", font_files2[s]);
            }
        }

SKIP_FONT:
        xf_otf_file_close (&otf_file);
        break;
    }
    return EXIT_SUCCESS;
}
