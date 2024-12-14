// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 FIXME
// Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree:
//   Copyright (c) 2013, The Linux Foundation. All rights reserved. (FIXME)

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>

#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <drm/drm_probe_helper.h>

struct s6e3fa9_amb667um06 {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	struct gpio_desc *reset_gpio;
};

static inline
struct s6e3fa9_amb667um06 *to_s6e3fa9_amb667um06(struct drm_panel *panel)
{
	return container_of(panel, struct s6e3fa9_amb667um06, panel);
}

static void s6e3fa9_amb667um06_reset(struct s6e3fa9_amb667um06 *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(10000, 11000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(10000, 11000);
}

static int s6e3fa9_amb667um06_on(struct s6e3fa9_amb667um06 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	ctx->dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	mipi_dsi_dcs_exit_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_usleep_range(&dsi_ctx, 10000, 11000);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x35, 0x00);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x2a,
					 0x00, 0x00, 0x04, 0x37);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0x2b,
					 0x00, 0x00, 0x09, 0x5f);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xfc, 0x5a, 0x5a);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xe5, 0x13);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xed, 0x00, 0x4c);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xfc, 0xa5, 0xa5);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xb9,
					 0x00, 0x80, 0xe8, 0x09, 0x00, 0x00,
					 0x00, 0x11, 0x03);
	mipi_dsi_generic_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);

	return dsi_ctx.accum_err;
}

static int s6e3fa9_amb667um06_off(struct s6e3fa9_amb667um06 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	ctx->dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	mipi_dsi_dcs_set_display_off_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 20);
	mipi_dsi_dcs_enter_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 120);

	return dsi_ctx.accum_err;
}

static int s6e3fa9_amb667um06_prepare(struct drm_panel *panel)
{
	struct s6e3fa9_amb667um06 *ctx = to_s6e3fa9_amb667um06(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	s6e3fa9_amb667um06_reset(ctx);

	ret = s6e3fa9_amb667um06_on(ctx);
	if (ret < 0) {
		dev_err(dev, "Failed to initialize panel: %d\n", ret);
		gpiod_set_value_cansleep(ctx->reset_gpio, 1);
		return ret;
	}

	return 0;
}

static int s6e3fa9_amb667um06_unprepare(struct drm_panel *panel)
{
	struct s6e3fa9_amb667um06 *ctx = to_s6e3fa9_amb667um06(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = s6e3fa9_amb667um06_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	gpiod_set_value_cansleep(ctx->reset_gpio, 1);

	return 0;
}

static const struct drm_display_mode s6e3fa9_amb667um06_mode = {
	.clock = (1080 + 120 + 121 + 122) * (2400 + 248 + 2 + 16) * 60 / 1000,
	.hdisplay = 1080,
	.hsync_start = 1080 + 120,
	.hsync_end = 1080 + 120 + 121,
	.htotal = 1080 + 120 + 121 + 122,
	.vdisplay = 2400,
	.vsync_start = 2400 + 248,
	.vsync_end = 2400 + 248 + 2,
	.vtotal = 2400 + 248 + 2 + 16,
	.width_mm = 70,
	.height_mm = 155,
	.type = DRM_MODE_TYPE_DRIVER,
};

static int s6e3fa9_amb667um06_get_modes(struct drm_panel *panel,
					struct drm_connector *connector)
{
	return drm_connector_helper_get_modes_fixed(connector, &s6e3fa9_amb667um06_mode);
}

static const struct drm_panel_funcs s6e3fa9_amb667um06_panel_funcs = {
	.prepare = s6e3fa9_amb667um06_prepare,
	.unprepare = s6e3fa9_amb667um06_unprepare,
	.get_modes = s6e3fa9_amb667um06_get_modes,
};

static int s6e3fa9_amb667um06_bl_update_status(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	u16 brightness = backlight_get_brightness(bl);
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_brightness_large(dsi, brightness);
	if (ret < 0)
		return ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	return 0;
}

// TODO: Check if /sys/class/backlight/.../actual_brightness actually returns
// correct values. If not, remove this function.
static int s6e3fa9_amb667um06_bl_get_brightness(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	u16 brightness;
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_get_display_brightness_large(dsi, &brightness);
	if (ret < 0)
		return ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	return brightness;
}

static const struct backlight_ops s6e3fa9_amb667um06_bl_ops = {
	.update_status = s6e3fa9_amb667um06_bl_update_status,
	.get_brightness = s6e3fa9_amb667um06_bl_get_brightness,
};

static struct backlight_device *
s6e3fa9_amb667um06_create_backlight(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	const struct backlight_properties props = {
		.type = BACKLIGHT_RAW,
		.brightness = 425,
		.max_brightness = 425,
	};

	return devm_backlight_device_register(dev, dev_name(dev), dev, dsi,
					      &s6e3fa9_amb667um06_bl_ops, &props);
}

static int s6e3fa9_amb667um06_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct s6e3fa9_amb667um06 *ctx;
	int ret;

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->reset_gpio),
				     "Failed to get reset-gpios\n");

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_CLOCK_NON_CONTINUOUS;

	drm_panel_init(&ctx->panel, dev, &s6e3fa9_amb667um06_panel_funcs,
		       DRM_MODE_CONNECTOR_DSI);
	ctx->panel.prepare_prev_first = true;

	ctx->panel.backlight = s6e3fa9_amb667um06_create_backlight(dsi);
	if (IS_ERR(ctx->panel.backlight))
		return dev_err_probe(dev, PTR_ERR(ctx->panel.backlight),
				     "Failed to create backlight\n");

	drm_panel_add(&ctx->panel);

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		drm_panel_remove(&ctx->panel);
		return dev_err_probe(dev, ret, "Failed to attach to DSI host\n");
	}

	return 0;
}

static void s6e3fa9_amb667um06_remove(struct mipi_dsi_device *dsi)
{
	struct s6e3fa9_amb667um06 *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id s6e3fa9_amb667um06_of_match[] = {
	{ .compatible = "mdss,s6e3fa9-amb667um06" }, // FIXME
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, s6e3fa9_amb667um06_of_match);

static struct mipi_dsi_driver s6e3fa9_amb667um06_driver = {
	.probe = s6e3fa9_amb667um06_probe,
	.remove = s6e3fa9_amb667um06_remove,
	.driver = {
		.name = "panel-s6e3fa9-amb667um06",
		.of_match_table = s6e3fa9_amb667um06_of_match,
	},
};
module_mipi_dsi_driver(s6e3fa9_amb667um06_driver);

MODULE_AUTHOR("linux-mdss-dsi-panel-driver-generator <fix@me>"); // FIXME
MODULE_DESCRIPTION("DRM driver for ss_dsi_panel_S6E3FA9_AMB667UM06_FHD");
MODULE_LICENSE("GPL");
