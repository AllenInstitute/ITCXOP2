#include "XOPStandardHeaders.r"

resource 'vers' (1) {						// XOP version info.
	0x01, 0x20, release, 0x00, 0,			// version bytes and country integer.
	"1.20",
	"1.20, Copyright 1993-2013 WaveMetrics, Inc., all rights reserved."
};

resource 'vers' (2) {						// Igor version info.
	0x06, 0x020, release, 0x00, 0,			// Version bytes and country integer.
	"6.20",
	"(for Igor 6.20 or later)"
};

// Custom error messages
resource 'STR#' (1100) {
	{
		/* [1] */
		"itcXOP2 requires Igor Pro 6.20 or later.",
	}
};

// No menu item

resource 'XOPI' (1100) {
	XOP_VERSION,							// XOP protocol version.
	DEV_SYS_CODE,							// Development system information.
	0,										// Obsolete - set to zero.
	0,										// Obsolete - set to zero.
	XOP_TOOLKIT_VERSION,					// XOP Toolkit version.
};

resource 'XOPC' (1100) {
	{
		"itcXOP2",								// Name of operation.
		waveOP+XOPOp+compilableOp,			// Operation's category.
	}
};


