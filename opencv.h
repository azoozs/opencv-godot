/* opencv.h */


#ifndef OPENCV_H
#define OPENCV_H


#include "core/object/ref_counted.h"

#include <core/io/image.h>
#include "scene/resources/image_texture.h"
#include <modules/dds/texture_loader_dds.h>

class opencv : public RefCounted  {
    GDCLASS(opencv, RefCounted );

protected:
    static void _bind_methods();

public:
	String get_opencv_version();
	Ref<ImageTexture> load_image_from_path(String path);
	Ref<ImageTexture> read_base64_img_as_opencv_img(String base64_string);

    // start video capture #################################################################################################
	Ref<ImageTexture> video_capture(int deviceID);
    Ref<ImageTexture> frame;
    Ref<ImageTexture> set_frame(Ref<ImageTexture> frame);
    Ref<ImageTexture> get_frame();
    bool is_video;
    bool set_video_bool(bool true_or_false);
    bool get_video_bool();
	// end video capture ###################################################################################################


    opencv();
};

#endif // OPENCV_H
