#ifndef __ALTERA_UP_AVALON_VIDEO_RGB_RESAMPLER_H__
#define __ALTERA_UP_AVALON_VIDEO_RGB_RESAMPLER_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// Decrease the number of bits
#define ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_TO_8BIT(c)		(((c >>  8) & 0x000000E0) | ((c >>  6) & 0x0000001C) | ((c >> 3) & 0x00000003))

#define ALT_UP_VIDEO_RESAMPLE_RGB_24BIT_TO_8BIT(c)		(((c >> 16) & 0x000000E0) | ((c >> 11) & 0x0000001C) | ((c >> 6) & 0x00000003))
#define ALT_UP_VIDEO_RESAMPLE_RGB_24BIT_TO_16BIT(c)		(((c >>  8) & 0x0000F800) | ((c >>  5) & 0x000007E0) | ((c >> 3) & 0x0000001F))

#define ALT_UP_VIDEO_RESAMPLE_RGB_30BIT_TO_8BIT(c)		(((c >> 16) & 0x000000E0) | ((c >> 11) & 0x0000001C) | ((c >> 6) & 0x00000003))
#define ALT_UP_VIDEO_RESAMPLE_RGB_30BIT_TO_16BIT(c)		(((c >>  8) & 0x0000F800) | ((c >>  5) & 0x000007E0) | ((c >> 3) & 0x0000001F))
#define ALT_UP_VIDEO_RESAMPLE_RGB_30BIT_TO_24BIT(c)		(((c >>  6) & 0x00FF0000) | ((c >>  4) & 0x0000FF00) | ((c >> 2) & 0x000000FF))

// Increase the number of bits
#define ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_TO_16BIT(c)		{ \
		ALT_UP_VIDEO_RESAMPLE_RGB_30BIT_TO_16BIT( \
			ALT_UP_VIDEO_RESAMPLE_RGB_TO_30BIT_RGB( \
				ALT_UP_VIDEO_RESAMPLE_RGB_3BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_R(c)), \
				ALT_UP_VIDEO_RESAMPLE_RGB_3BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_G(c)), \
				ALT_UP_VIDEO_RESAMPLE_RGB_2BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_B(c))  \
			) \
		) \
	}
#define ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_TO_24BIT(c)		{ \
		ALT_UP_VIDEO_RESAMPLE_RGB_30BIT_TO_24BIT( \
			ALT_UP_VIDEO_RESAMPLE_RGB_TO_30BIT_RGB( \
				ALT_UP_VIDEO_RESAMPLE_RGB_3BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_R(c)), \
				ALT_UP_VIDEO_RESAMPLE_RGB_3BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_G(c)), \
				ALT_UP_VIDEO_RESAMPLE_RGB_2BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_B(c))  \
			) \
		) \
	}
#define ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_TO_30BIT(c)		{ \
		ALT_UP_VIDEO_RESAMPLE_RGB_TO_30BIT_RGB( \
			ALT_UP_VIDEO_RESAMPLE_RGB_3BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_R(c)), \
			ALT_UP_VIDEO_RESAMPLE_RGB_3BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_G(c)), \
			ALT_UP_VIDEO_RESAMPLE_RGB_2BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_B(c))  \
		) \
	}

#define ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_TO_24BIT(c)		{ \
		ALT_UP_VIDEO_RESAMPLE_RGB_30BIT_TO_24BIT( \
			ALT_UP_VIDEO_RESAMPLE_RGB_TO_30BIT_RGB( \
				ALT_UP_VIDEO_RESAMPLE_RGB_5BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_EXTRACT_R(c)), \
				ALT_UP_VIDEO_RESAMPLE_RGB_6BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_EXTRACT_G(c)), \
				ALT_UP_VIDEO_RESAMPLE_RGB_5BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_EXTRACT_B(c))  \
			) \
		) \
	}
#define ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_TO_30BIT(c)		{ \
		ALT_UP_VIDEO_RESAMPLE_RGB_TO_30BIT_RGB( \
			ALT_UP_VIDEO_RESAMPLE_RGB_5BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_EXTRACT_R(c)), \
			ALT_UP_VIDEO_RESAMPLE_RGB_6BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_EXTRACT_G(c)), \
			ALT_UP_VIDEO_RESAMPLE_RGB_5BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_EXTRACT_B(c))  \
		) \
	}

#define ALT_UP_VIDEO_RESAMPLE_RGB_24BIT_TO_30BIT(c)		{ \
		ALT_UP_VIDEO_RESAMPLE_RGB_TO_30BIT_RGB( \
			ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_24BIT_EXTRACT_R(c)), \
			ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_24BIT_EXTRACT_G(c)), \
			ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_TO_10BIT(ALT_UP_VIDEO_RESAMPLE_RGB_24BIT_EXTRACT_B(c))  \
		) \
	}




// Helper macros
#define ALT_UP_VIDEO_RESAMPLE_RGB_2BIT_TO_10BIT(c)		((c << 8) | (c << 6) | (c << 4) | (c << 2) | (c << 0))
#define ALT_UP_VIDEO_RESAMPLE_RGB_3BIT_TO_10BIT(c)		((c << 7) | (c << 4) | (c << 1) | (c >> 2))
#define ALT_UP_VIDEO_RESAMPLE_RGB_5BIT_TO_10BIT(c)		((c << 5) | (c << 0))
#define ALT_UP_VIDEO_RESAMPLE_RGB_6BIT_TO_10BIT(c)		((c << 4) | (c >> 2))
#define ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_TO_10BIT(c)		((c << 2) | (c >> 6))

#define ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_R(c)		((c >>  5) & 0x07)
#define ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_G(c)		((c >>  2) & 0x02)
#define ALT_UP_VIDEO_RESAMPLE_RGB_8BIT_EXTRACT_B(c)		((c >>  0) & 0x00)

#define ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_EXTRACT_R(c)	((c >> 11) & 0x1F)
#define ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_EXTRACT_G(c)	((c >>  5) & 0x3F)
#define ALT_UP_VIDEO_RESAMPLE_RGB_16BIT_EXTRACT_B(c)	((c >>  0) & 0x1F)

#define ALT_UP_VIDEO_RESAMPLE_RGB_24BIT_EXTRACT_R(c)	((c >> 16) & 0xFF)
#define ALT_UP_VIDEO_RESAMPLE_RGB_24BIT_EXTRACT_G(c)	((c >>  8) & 0xFF)
#define ALT_UP_VIDEO_RESAMPLE_RGB_24BIT_EXTRACT_B(c)	((c >>  0) & 0xFF)

#define ALT_UP_VIDEO_RESAMPLE_RGB_TO_30BIT_RGB(r,g,b)	((r << 20) | (b << 10) | (b << 0))

/*
 * Device structure definition. Each instance of the driver uses one
 * of these structures to hold its associated state.
 */
typedef struct alt_up_video_rgb_resampler_dev {
	/// @brief character mode device structure 
	/// @sa Developing Device Drivers for the HAL in Nios II Software Developer's Handbook
	alt_dev dev;
	/// @brief the video rgb resampler's slave base address
	unsigned int base;
	/// @brief the stream-in's mode
	unsigned int stream_in_mode;
	/// @brief Is the stream-in in color?
	unsigned int stream_in_color;
	/// @brief Does the stream-in use alpha?
	unsigned int stream_in_alpha;
	/// @brief the stream-out's mode
	unsigned int stream_out_mode;
	/// @brief Is the stream-out in color?
	unsigned int stream_out_color;
	/// @brief Does the stream-out use alpha?
	unsigned int stream_out_alpha;
} alt_up_video_rgb_resampler_dev;

///////////////////////////////////////////////////////////////////////////////
// HAL system functions

///////////////////////////////////////////////////////////////////////////////
// file-like operation functions

///////////////////////////////////////////////////////////////////////////////
// direct operation functions
/**
 * @brief Opens the video rgb resampler device specified by <em> name </em>
 *
 * @param name -- the video rgb resampler component name in Qsys 
 *
 * @return The corresponding device structure, or NULL if the device is not found
 **/
alt_up_video_rgb_resampler_dev* alt_up_video_rgb_resampler_open_dev(const char* name);

///////////////////////////////////////////////////////////////////////////////
// Macros used by alt_sys_init 
#define ALTERA_UP_AVALON_VIDEO_RGB_RESAMPLER_INSTANCE(name, device)		\
static alt_up_video_rgb_resampler_dev device =							\
{																		\
	{																	\
		ALT_LLIST_ENTRY,												\
		name##_NAME,													\
		NULL, /* open  */												\
		NULL, /* close */												\
		NULL, /* read  */												\
		NULL, /* write */												\
		NULL, /* lseek */												\
		NULL, /* fstat */												\
		NULL, /* ioctl */												\
	},																	\
	name##_BASE,														\
	0,		/* Default stream-in's mode				*/					\
	0,		/* Default stream-in's color enabled	*/					\
	0,		/* Default stream-in's alpha enabled	*/					\
	0,		/* Default stream-out's mode			*/					\
	0,		/* Default stream-out's color enabled	*/					\
	0,		/* Default stream-out's alpha enabled	*/					\
}

#define ALTERA_UP_AVALON_VIDEO_RGB_RESAMPLER_INIT(name, device)			\
{																		\
	device.stream_in_mode =												\
		(*((int *)(device.base)) & 0x0000000F);							\
	device.stream_in_color =											\
		((*((int *)(device.base)) >>  4) & 0x00000001);					\
	device.stream_in_alpha =											\
		((*((int *)(device.base)) >>  5) & 0x00000001);					\
	device.stream_out_mode =											\
		((*((int *)(device.base)) >> 16) & 0x0000000F);					\
	device.stream_out_color =											\
		((*((int *)(device.base)) >> 20) & 0x00000001);					\
	device.stream_out_alpha =											\
		((*((int *)(device.base)) >> 21) & 0x00000001);					\
																		\
	/* make the device available to the system */						\
	alt_dev_reg(&device.dev);											\
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ALTERA_UP_AVALON_VIDEO_RGB_RESAMPLER_H__ */


