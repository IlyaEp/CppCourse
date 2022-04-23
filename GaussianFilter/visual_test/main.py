import argparse
import cv2
import os


def read_image(image_path):
    return cv2.imread(image_path, cv2.BORDER_DEFAULT)


def blur(image_src, filter_size):
    blured_image = cv2.GaussianBlur(image_src, filter_size, cv2.BORDER_DEFAULT)
    border_size = filter_size[0]
    return cv2.copyMakeBorder(
        blured_image[border_size:-border_size, border_size:-border_size],
        top=border_size,
        bottom=border_size,
        left=border_size,
        right=border_size,
        borderType=cv2.BORDER_CONSTANT,
        value=[0, 0, 0])


def _main():
    parser = argparse.ArgumentParser(description='Test blur images with OpenCV.')
    parser.add_argument('-i', '--input', required=True, type=str, help='Input directory with img images.')
    parser.add_argument('-o', '--output', required=True, type=str, help='Output directory with blurred images.')
    parser.add_argument('-r', '--radius', required=True, type=int, help='Filter radius')

    args = parser.parse_args()

    input_dir = args.input
    output_dir = args.output
    filter_radius = args.radius

    for image_filename in os.listdir(input_dir):
        if not image_filename.endswith(".bmp"):
            continue
        src_image = read_image(os.path.join(input_dir, image_filename))
        dst_image = blur(src_image, filter_size=(filter_radius, filter_radius))
        cv2.imwrite(os.path.join(output_dir, image_filename), dst_image)


if __name__ == '__main__':
    exit(_main())
