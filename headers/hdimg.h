#pragma once
#ifndef HDIMG_DECLA_H
#define HDIMG_DECLA_H

#define EXT extern
#define INIT(s)

#if !defined(SSE_ACSI_DISABLE_HDIMG)

EXT void hdimg_init_vectors();
EXT void hdimg_reset();
EXT void hdimg_intercept(MEM_ADDRESS);
EXT void hdimg_intercept_init();

EXT bool hdimg_active INIT(0);

EXT MEM_ADDRESS os_hdimg_init_vector INIT(0),os_hdimg_bpb_vector INIT(0),os_hdimg_rw_vector INIT(0),
              os_hdimg_boot_vector INIT(0),os_hdimg_mediach_vector INIT(0);

#endif//#if !defined(SSE_ACSI_DISABLE_HDIMG)

#undef EXT
#undef INIT

#endif//HDIMG_DECLA_H