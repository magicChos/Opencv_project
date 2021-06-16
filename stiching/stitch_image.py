import sys
import cv2
import numpy as np


# draw epipolar line
def drawlines(img1 , img2 , lines , pts1 , pts2):
    r , c = img1.shape
    if img1.shape[2] == 1:
        img1 = cv2.cvtColor(img1 , cv2.COLOR_GRAY2BGR)
    if img2.shape[2] == 1:
        img2 = cv2.cvtColor(img2 , cv2.COLOR_GRAY2BGR)
    for r, pt1, pt2 in zip(lines, pts1, pts2):
        color = tuple(np.random.randint(0, 255, 3).tolist())
        x0, y0 = map(int, [0, -r[2] / r[1]])
        x1, y1 = map(int, [c, -(r[2] + r[0] * c) / r[1]])
        img1 = cv2.line(img1, (x0, y0), (x1, y1), color, 1)
        img1 = cv2.circle(img1, tuple(pt1), 5, color, -1)
        img2 = cv2.circle(img2, tuple(pt2), 5, color, -1)
    return img1, img2

# Use the keypoints to stitch the images
def get_stitched_image(img1, img2, M):
    # Get width and height of input images
    h1, w1 = img1.shape[:2]
    h2, w2 = img2.shape[:2]

    # Get the canvas dimesions
    img1_dims = np.float32([[0, 0], [0, h1], [w1, h1], [w1, 0]]).reshape(-1, 1, 2)
    img2_dims_temp = np.float32([[0, 0], [0, h2], [w2, h2], [w2, 0]]).reshape(-1, 1, 2)

    # Get relative perspective of second image
    img2_dims = cv2.perspectiveTransform(img2_dims_temp, M)
    print('img2_dims:' , img2_dims)
    print('-----------------------')
    ds = np.dot(M , np.array([0 , 0 , 1]))
    print('ds:' , ds/ds[-1])


    # Resulting dimensions
    result_dims = np.concatenate((img1_dims, img2_dims), axis=0)

    # Getting images together
    # Calculate dimensions of match points
    [x_min, y_min] = np.int32(result_dims.min(axis=0).ravel() - 0.5)
    [x_max, y_max] = np.int32(result_dims.max(axis=0).ravel() + 0.5)

    # Create output array after affine transformation
    transform_dist = [-x_min, -y_min]

    transform_array = np.array([[1, 0, transform_dist[0]],
                                [0, 1, transform_dist[1]],
                                [0, 0, 1]])

    # Warp images to get the resulting image
    result_img = cv2.warpPerspective(img2, transform_array.dot(M),
                                     (x_max - x_min, y_max - y_min))
    result_img[transform_dist[1]:h1 + transform_dist[1],transform_dist[0]:w1 + transform_dist[0]] = img1

    # Return the result
    return result_img


# Find SIFT and return Homography Matrix
def get_sift_homography(img1, img2):
    # Initialize SIFT
    # sift = cv2.xfeatures2d.SIFT_create()
    sift = cv2.xfeatures2d.SURF_create()

    # Extract keypoints and descriptors
    k1, d1 = sift.detectAndCompute(img1, None)
    k2, d2 = sift.detectAndCompute(img2, None)

    # Bruteforce matcher on the descriptors
    bf = cv2.BFMatcher()
    matches1 = bf.knnMatch(d1, d2, k=2)

    print(type(matches1))

    # Make sure that the matches are good
    verify_ratio = 0.6  # Source: stackoverflow
    verified_matches1 = []

    for m1, m2 in matches1:
        # Add to array only if it's a good match
        if m1.distance < verify_ratio * m2.distance:
            verified_matches1.append(m1)

    matches2 = bf.knnMatch(d2 , d1 , k=2)
    verified_matches2 = []
    for m1, m2 in matches2:
        if m1.distance < verify_ratio * m2.distance:
            verified_matches2.append(m1)

    # symmetry test
    verified_matches = []
    for sym1 in verified_matches1:
        for sym2 in verified_matches2:
            if sym1.queryIdx == sym2.trainIdx and sym1.trainIdx == sym2.queryIdx:
                verified_matches.append(sym1)

    print('verified_matches number:' , len(verified_matches))

    min_matches = 8
    if len(verified_matches) > min_matches:

        # Array to store matching points
        img1_pts = []
        img2_pts = []

        # Add matching points to array
        for match in verified_matches:
            img1_pts.append(k1[match.queryIdx].pt)
            img2_pts.append(k2[match.trainIdx].pt)
        img1_pts = np.float32(img1_pts).reshape(-1, 1, 2)
        img2_pts = np.float32(img2_pts).reshape(-1, 1, 2)

        F , mask = cv2.findFundamentalMat(img1_pts , img2_pts , cv2.RANSAC , 2)
        img1_pts_inlier = img1_pts[mask.ravel() == 1]
        img2_pts_inlier = img2_pts[mask.ravel() == 1]

        index = np.where(mask.ravel() == 1)

        best_matches = [verified_matches[i] for i in index[0]]
        print('best_matches:' , len(best_matches))

        outImg = None
        outImg = cv2.drawMatches(img1 , k1 , img2 , k2 , best_matches , img2 , flags=2)
        cv2.imshow("outImg" , outImg)
        cv2.waitKey(0)


        match_img = None


        # Compute homography matrix
        M, mask = cv2.findHomography(img1_pts_inlier, img2_pts_inlier, cv2.RANSAC, 3.0)
        # print('M:' , M)
        return M
    else:
        print 'Error: Not enough matches'
        exit()


# Equalize Histogram of Color Images
def equalize_histogram_color(img):
    img_yuv = cv2.cvtColor(img, cv2.COLOR_BGR2YUV)
    img_yuv[:, :, 0] = cv2.equalizeHist(img_yuv[:, :, 0])
    img = cv2.cvtColor(img_yuv, cv2.COLOR_YUV2BGR)
    return img


# Main function definition
def main():
    # Get input set of images
    img1 = cv2.imread(sys.argv[1])
    img2 = cv2.imread(sys.argv[2])

    img11 = cv2.pyrDown(img1)
    img22 = cv2.pyrDown(img2)


    # Equalize histogram
    Eq_img11 = equalize_histogram_color(img11)
    Eq_img22 = equalize_histogram_color(img22)

    # Use SIFT to find keypoints and return homography matrix
    M = get_sift_homography(Eq_img11, Eq_img22)

    # Stitch the images together using homography matrix
    result_image = get_stitched_image(img22, img11, M)

    cv2.imwrite("result.jpg" , result_image)

# Call main function
if __name__ == '__main__':
    main()