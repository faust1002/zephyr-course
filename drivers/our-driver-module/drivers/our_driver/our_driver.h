#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_set(const struct device* dev,
                   int new_value);

#ifdef __cplusplus
}
#endif
