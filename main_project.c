#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <curl/curl.h>

#define MAX_LINE_LENGTH 256
//----------------------------------------------------------------------------------------------------

void input_way(int* n);
unsigned char* open_image_by_path(const char* filename, int* width, int* height, int* channels);
void download_image(const char* url, const char* filename);
char* open_image_by_email();
int save_image(const char* filename, int width, int height, int channels, unsigned char* data);
int find_way(int way, const char* filename, int* width, int* height, int* channels);
void witch_filter(int* n);
void Apply_filter(int filter, unsigned char* data, int width, int height, int channels);
const char* get_image_format();
void rgb_to_grayscale(unsigned char* data, int width, int height);
void rgba_to_rgb(unsigned char* data, int width, int height);
void BW(unsigned char* data, int width, int height, int channels);
void sharp(unsigned char* data, int width, int height, int channels);
void sepia(unsigned char* data, int width, int height, int channels);
void gaussian_blur(unsigned char* data, int width, int height, int channels);
void edge_detection(unsigned char* data, int width, int height, int channels);
void saturation(unsigned char* data, int width, int height, int channels);
void emboss(unsigned char* data, int width, int height, int channels);
void invert_colors(unsigned char* data, int width, int height, int channels);
void infraredFilter(unsigned char* data, int width, int height, int channels);
void distort(unsigned char* data, int width, int height, int channels);
void noise(unsigned char* data, int width, int height, int channels);
void pixelate(unsigned char* data, int width, int height, int channels);
void render(unsigned char* data, int width, int height, int channels);
void stylize(unsigned char* data, int width, int height, int channels);
void sketch(unsigned char* data, int width, int height, int channels);
void artisticFilter(unsigned char* data, int width, int height, int channels);
void textureFilter(unsigned char* data, int width, int height, int channels);
void vscoFilter(unsigned char* data, int width, int height, int channels);
void VintageFilter(unsigned char* data, int width, int height, int channels);
void RetroFilter(unsigned char* data, int width, int height, int channels);
void GrayscaleFilter(unsigned char* data, int width, int height, int channels);

//----------------------------------------------------------------------------------------------------

int main() {
    int way;
    input_way(&way);
    if (way == 1) {
        printf("Please enter the path of your image :\n");
        char* input_filename = malloc(1000 * sizeof(char));
        scanf(" %[^\n]", input_filename);
        int width, height, channels;
        find_way(way, input_filename, &width, &height, &channels);
        unsigned char* image_data = open_image_by_path(input_filename, &width, &height, &channels);
        if (image_data != NULL) {
            int filter;
            witch_filter(&filter);
            Apply_filter(filter, image_data, width, height, channels);
            printf("Please enter the path you want to save image.\n");
            char* output_filename = malloc(1000 * sizeof(char));
            scanf(" %[^\n]", output_filename);
            const char* format = get_image_format();
            char* filename_with_format = malloc((strlen(output_filename) + strlen(format) + 2) * sizeof(char));
            snprintf(filename_with_format, strlen(output_filename) + strlen(format) + 2, "%s.%s", output_filename, format);
            int save_result = save_image(filename_with_format, width, height, channels, image_data);
            if (save_result == 0) {
                printf("\nImage saved successfully!\n");
            } else {
                printf("\nFailed to save image!\n");
            }
            stbi_image_free(image_data);
            free(filename_with_format);
        }
        free(input_filename);
    } else if (way == 2) {
        printf("Please enter the URL of your image:\n");
        char url[1000];
        scanf(" %[^\n]", url);
        printf("Please enter the path you want to save the image :\n");
        char output_filename[1000];
        scanf(" %[^\n]", output_filename);
        download_image(url, output_filename);
        int width, height, channels;
        unsigned char* image_data = open_image_by_path(output_filename, &width, &height, &channels);
        if (image_data != NULL) {
            int filter;
            witch_filter(&filter);
            Apply_filter(filter, image_data, width, height, channels);
            printf("Please enter the path you want to save image.\n");
            char* output_filename = malloc(1000 * sizeof(char));
            scanf(" %[^\n]", output_filename);
            const char* format = get_image_format();
            char* filename_with_format = malloc((strlen(output_filename) + strlen(format) + 2) * sizeof(char));
            snprintf(filename_with_format, strlen(output_filename) + strlen(format) + 2, "%s.%s", output_filename, format);
            int save_result = save_image(filename_with_format, width, height, channels, image_data);
            if (save_result == 0) {
                printf("\nImage saved successfully!\n");
            } else {
                printf("\nFailed to save image!\n");
            }
            stbi_image_free(image_data);
            free(filename_with_format);
        }
    }else if(way == 3){
        char* image = open_image_by_email();
        int width, height, channels;
        unsigned char* image_data = open_image_by_path(image, &width, &height, &channels);
        if (image_data != NULL) {
            int filter;
            witch_filter(&filter);
            Apply_filter(filter, image_data, width, height, channels);
            printf("Please enter the path you want to save image.\n");
            char* output_filename = malloc(1000 * sizeof(char));
            scanf(" %[^\n]", output_filename);
            const char* format = get_image_format();
            char* filename_with_format = malloc((strlen(output_filename) + strlen(format) + 2) * sizeof(char));
            snprintf(filename_with_format, strlen(output_filename) + strlen(format) + 2, "%s.%s", output_filename, format);
            int save_result = save_image(filename_with_format, width, height, channels, image_data);
            if (save_result == 0) {
                printf("\nImage saved successfully!\n");
            } else {
                printf("\nFailed to save image!\n");
            }
            stbi_image_free(image_data);
            free(filename_with_format);
        }
        free(image);
    }
    return 0;
}
//----------------------------------------------------------------------------------------------------

void input_way(int* n) {
    printf("Which way would you like to open an image?\n1. by its path.\n2. by its URL.\n3. by emailing the image.\n");
    scanf("%d", n);
}

//----------------------------------------------------------------------------------------------------

unsigned char* open_image_by_path(const char* filename, int* width, int* height, int* channels) {
    int w, h, c;
    unsigned char* image_data = stbi_load(filename, &w, &h, &c, 0);
    if (!image_data) {
        printf("Failed to open image: %s\n", filename);
        return NULL;
    }
    *width = w;
    *height = h;
    *channels = c;
    return image_data;

}


//----------------------------------------------------------------------------------------------------

void download_image(const char* url, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Failed to open the file for writing.\n");
        return;
    }

    // Create the command to download the image using curl
    char command[256];
    snprintf(command, sizeof(command), "curl -o \"%s\" \"%s\"", filename, url);

    // Execute the command using the system function
    int result = system(command);

    if (result == 0) {
        printf("Image downloaded successfully.\n");
    } else {
        printf("Failed to download the image.\n");
    }

    fclose(file);
}

//----------------------------------------------------------------------------------------------------


int extractTextFromFile(const char* inputFilename, const char* outputFilename) {
    FILE *inputFile, *outputFile;
    char line[MAX_LINE_LENGTH];
    char startString[] = "X-Attachment-Id:";
    char endString[] = "--00000";
    int foundStart = 0;
    int foundEnd = 0;

    // Open input file for reading
    inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    // Open output file for writing
    outputFile = fopen(outputFilename, "a");
    if (outputFile == NULL) {
        printf("Error opening output file.\n");
        fclose(inputFile);
        return 1;
    }

    // Search for the start string and write text until the end string to the output file
    while (fgets(line, sizeof(line), inputFile)) {
        if (!foundStart) {
            if (strncmp(line, startString, strlen(startString)) == 0) {
                foundStart = 1; // Found the start string, start writing lines to the output file
            }
        } else {
            if (strstr(line, endString) != NULL) {
                foundEnd = 1; // Found the end string, stop writing lines to the output file
                break;
            }
            fputs(line, outputFile); // Write the line to the output file
        }
    }

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    if (foundStart && foundEnd) {
        printf("Text between '%s' and '%s' has been written to '%s'.\n", startString, endString, outputFilename);
    } else {
        printf("Start or end string not found.\n");
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------

// Callback function to write attachment data to file
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    FILE* file = (FILE*)userp;
    size_t written = fwrite(contents, size, nmemb, file);
    return written;
}

//----------------------------------------------------------------------------------------------------

int fetchEmailAndSaveToFile(const char* email, const char* password, const char* mailbox, int uid, const char* outputFilename) {
    CURL* curl;
    CURLcode res;
    FILE* file;

    // Set up libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        // Create the URL with the UID
        char url[100];
        sprintf(url, "imaps://imap.gmail.com/%s;UID=%d", mailbox, uid);

        // Connect to the IMAP server and log in
        curl_easy_setopt(curl, CURLOPT_USERNAME, email);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Perform the fetch request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Failed to fetch email: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return 1;
        }

        // Open a file to save the fetched email
        file = fopen(outputFilename, "wb");
        if (!file) {
            printf("Error opening file for writing.\n");
            curl_easy_cleanup(curl);
            return 1;
        }

        // Set the write callback function to write data into the file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        // Perform the fetch request and save the email content to the file
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Failed to fetch email: %s\n", curl_easy_strerror(res));
            fclose(file);
            curl_easy_cleanup(curl);
            return 1;
        }

        // Clean up and close the file
        fclose(file);

        // Clean up and close the IMAP session
        curl_easy_cleanup(curl);
    } else {
        printf("Error initializing libcurl.\n");
        return 1;
    }

    // Clean up libcurl
    curl_global_cleanup();

    return 0;
}

//----------------------------------------------------------------------------------------------------

void decodeFile(const char* inputFile, const char* outputFile) {
    char command[256];
    sprintf(command, "certutil -decode %s %s", inputFile, outputFile);
    system(command);
}

//----------------------------------------------------------------------------------------------------
char* open_image_by_email(){
    const char email[] = "salarmahyar0001@gmail.com";
    const char password[] = "jtlmchhxwpcyirbj";
    const char mailbox[] = "INBOX";
    int uid = 2;
    const char outputFilename[] = "C:\\Users\\beta\\Desktop\\Apps\\coding_notes\\ctt2.txt";

    int result = fetchEmailAndSaveToFile(email, password, mailbox, uid, outputFilename);
    if (result == 0) {
        printf("Email fetched and saved to '%s'.\n", outputFilename);
    } else {
        printf("Failed to fetch and save email.\n");
    }
    const char* inp = outputFilename;
    const char* out = "C:\\Users\\beta\\Desktop\\Apps\\coding_notes\\ctt.txt";

    extractTextFromFile(inp,out);

    const char* image_path = "C:\\Users\\beta\\Downloads\\image_url\\image44.jpg";
    decodeFile(out,image_path);
    return image_path;
}
//----------------------------------------------------------------------------------------------------

const char* get_image_format() {
    printf("Please enter the desired image format (e.g., jpg, png, bmp):\n");
    char format[10];
    scanf("%s", format);

    // Allocate memory for the format string
    char* image_format = (char*)malloc(strlen(format) + 1);
    if (image_format == NULL) {
        printf("Failed to allocate memory for image format.\n");
        return NULL;
    }

    // Copy the format string
    strcpy(image_format, format);

    return image_format;
}



// Function to convert RGB to grayscale
void rgb_to_grayscale(unsigned char* data, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        uint8_t r = data[i * 3];
        uint8_t g = data[i * 3 + 1];
        uint8_t b = data[i * 3 + 2];
        uint8_t gray = (r + g + b) / 3;
        data[i * 3] = gray;
        data[i * 3 + 1] = gray;
        data[i * 3 + 2] = gray;
    }
}

// Function to convert RGBA to RGB by removing alpha channel
void rgba_to_rgb(unsigned char* data, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        data[i * 3] = data[i * 4];
        data[i * 3 + 1] = data[i * 4 + 1];
        data[i * 3 + 2] = data[i * 4 + 2];
    }
}

int save_image(const char* filename, int width, int height, int channels, unsigned char* data) {
    // Get the desired file extension from the filename
    const char* extension = strrchr(filename, '.');
    if (extension == NULL) {
        printf("Invalid file extension.\n");
        return 1;
    }

    // Determine the output image type based on the file extension
    int output_channels = (strcmp(extension, ".bmp") == 0) ? 3 : channels;

    // Convert the image data if necessary
    unsigned char* output_data = data;
    if (channels == 4 && output_channels == 3) {
        rgba_to_rgb(data, width, height);
    } else if (channels == 3 && output_channels == 1) {
        output_data = (unsigned char*)malloc(width * height);
        if (output_data == NULL) {
            printf("Failed to allocate memory for image data.\n");
            return 1;
        }
        rgb_to_grayscale(data, width, height);
    }

    // Save the image using the STB library
    int save_result = 0;
    if (output_channels == 1) {
        save_result = stbi_write_png(filename, width, height, output_channels, output_data, width);
    } else {
        save_result = stbi_write_bmp(filename, width, height, output_channels, output_data);
    }

    // Free the memory allocated for the converted image data
    if (output_data != data) {
        free(output_data);
    }

    if (save_result == 0) {
        printf("Failed to save image: %s\n", filename);
        return 1;
    }

    printf("Image saved successfully!\n");
    return 0;
}

//----------------------------------------------------------------------------------------------------

int find_way(int way, const char* filename, int* width, int* height, int* channels) {
    unsigned char* image_data;
    switch (way) {
        case 1:
            image_data = open_image_by_path(filename, width, height, channels);
            if (image_data == NULL) {
                return 1;
            }
            break;

        default:
            printf("Error: Invalid option selected!\n");
            return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------

void witch_filter(int* n) {
    printf("Which filter would you like to apply?\n"
           "1. Black and White\t\t2. Sharp\n"
           "3. Sepia\t\t\t4. Gaussian Blur\n"
           "5. Edge Detection\t\t6. Saturation\n"
           "7. Emboss\t\t\t8. Invert Colors\n"
           "9. Infrared Filter\t\t10. Distort\n"
           "11. Noise\t\t\t12. Pixelate\n"
           "13. Render\t\t\t14. Stylize\n"
           "15. Sketch\t\t\t16. Artistic\n"
           "17. Texture\t\t\t18. Vsco Filter\n"
           "19. Vintage Filter\t\t20. Retro Filter\n"
           "21. Grayscale Filter\n");
    scanf("%d", n);
}

//----------------------------------------------------------------------------------------------------

void Apply_filter(int filter, unsigned char* data, int width, int height, int channels) {
    switch (filter) {
        case 1:
            BW(data, width, height, channels);
            break;
        case 2:
            sharp(data, width, height, channels);
            break;
        case 3:
            sepia(data, width, height, channels);
            break;
        case 4:
            gaussian_blur(data, width, height, channels);
            break;
        case 5:
            edge_detection(data, width, height, channels);
            break;
        case 6:
            saturation(data, width, height, channels);
            break;
        case 7:
            emboss(data, width, height, channels);
            break;
        case 8:
            invert_colors(data, width, height, channels);
            break;
        case 9:
            infraredFilter(data, width, height, channels);
            break;
        case 10:
            distort(data, width, height, channels);
            break;
        case 11:
            noise(data, width, height, channels);
            break;
        case 12:
            pixelate(data, width, height, channels);
            break;
        case 13:
            render(data, width, height, channels);
            break;
        case 14:
            stylize(data, width, height, channels);
            break;
        case 15:
            sketch(data, width, height, channels);
            break;
        case 16:
            artisticFilter(data, width, height, channels);
            break;
        case 17:
            textureFilter(data, width, height, channels);
            break;
        case 18:
            vscoFilter(data, width, height, channels);
            break;
        case 19:
            VintageFilter(data, width, height, channels);
            break;
        case 20:
            RetroFilter(data, width, height, channels);
            break;
        case 21:
            GrayscaleFilter(data, width, height, channels);
            break;
        default:
            printf("Error: Invalid filter option!\n");
            break;
    }
}

//----------------------------------------------------------------------------------------------------

void BW(unsigned char* data, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; i += channels) {
        unsigned char r = data[i];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];
        unsigned char gray = (r + g + b) / 3;
        data[i] = gray;
        data[i + 1] = gray;
        data[i + 2] = gray;
    }
}

//----------------------------------------------------------------------------------------------------

void sharp(unsigned char* data, int width, int height, int channels) {
    unsigned char* temp_data = (unsigned char*)malloc(width * height * channels);
    if (temp_data == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    memcpy(temp_data, data, width * height * channels);

    int kernel[3][3] = {
            {-1, -1, -1},
            {-1, 9, -1},
            {-1, -1, -1}
    };

    int padding = 1;

    for (int y = padding; y < height - padding; y++) {
        for (int x = padding; x < width - padding; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = 0;
                for (int ky = -padding; ky <= padding; ky++) {
                    for (int kx = -padding; kx <= padding; kx++) {
                        int image_index = (y + ky) * width * channels + (x + kx) * channels + c;
                        int kernel_value = kernel[ky + padding][kx + padding];
                        sum += temp_data[image_index] * kernel_value;
                    }
                }
                int result = sum > 255 ? 255 : (sum < 0 ? 0 : sum);
                data[y * width * channels + x * channels + c] = (unsigned char)result;
            }
        }
    }

    free(temp_data);
}

//----------------------------------------------------------------------------------------------------

void sepia(unsigned char* data, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; i += channels) {
        unsigned char r = data[i];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];

        unsigned char new_r = (r * 0.393) + (g * 0.769) + (b * 0.189);
        unsigned char new_g = (r * 0.349) + (g * 0.686) + (b * 0.168);
        unsigned char new_b = (r * 0.272) + (g * 0.534) + (b * 0.131);

        data[i] = (new_r > 255) ? 255 : new_r;
        data[i + 1] = (new_g > 255) ? 255 : new_g;
        data[i + 2] = (new_b > 255) ? 255 : new_b;
    }
}

//----------------------------------------------------------------------------------------------------

void gaussian_blur(unsigned char* data, int width, int height, int channels) {
    // Create a temporary copy of the image data
    unsigned char* temp_data = (unsigned char*)malloc(width * height * channels);
    memcpy(temp_data, data, width * height * channels);

    // Define the Gaussian blur kernel
    float kernel[5][5] = {
            {1,  4,  7,  4, 1},
            {4, 16, 26, 16, 4},
            {7, 26, 41, 26, 7},
            {4, 16, 26, 16, 4},
            {1,  4,  7,  4, 1}
    };
    float kernel_sum = 273.0f; // Sum of all kernel values

    // Apply the Gaussian blur filter
    for (int y = 2; y < height - 2; y++) {
        for (int x = 2; x < width - 2; x++) {
            for (int c = 0; c < channels; c++) {
                float blur_value = 0.0f;
                for (int i = -2; i <= 2; i++) {
                    for (int j = -2; j <= 2; j++) {
                        int index = ((y + i) * width + (x + j)) * channels + c;
                        float kernel_value = kernel[i + 2][j + 2];
                        blur_value += temp_data[index] * kernel_value;
                    }
                }
                int current_index = (y * width + x) * channels + c;
                data[current_index] = (unsigned char)(blur_value / kernel_sum);
            }
        }
    }

    free(temp_data);
}


//----------------------------------------------------------------------------------------------------

void edge_detection(unsigned char* data, int width, int height, int channels) {
    // Convert the image to grayscale
    unsigned char* gray_data = (unsigned char*)malloc(width * height);

    for (int i = 0, j = 0; i < width * height * channels; i += channels, j++) {
        unsigned char r = data[i];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];

        gray_data[j] = (unsigned char)((r + g + b) / 3);
    }

    // Apply the edge detection filter
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            int current_index = y * width + x;
            unsigned char current_pixel = gray_data[current_index];

            int right_index = y * width + (x + 1);
            unsigned char right_pixel = gray_data[right_index];

            int bottom_index = (y + 1) * width + x;
            unsigned char bottom_pixel = gray_data[bottom_index];

            unsigned char edge_value = abs(current_pixel - right_pixel) + abs(current_pixel - bottom_pixel);
            data[current_index * channels] = edge_value;
            data[current_index * channels + 1] = edge_value;
            data[current_index * channels + 2] = edge_value;
        }
    }

    free(gray_data);
}

//----------------------------------------------------------------------------------------------------

void saturation(unsigned char* data, int width, int height, int channels) {
    const float saturation_factor = 2.0f; // Adjust this factor to change the saturation level

    for (int i = 0; i < width * height * channels; i += channels) {
        unsigned char r = data[i];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];

        float avg = (r + g + b) / 3.0f;
        float new_r = avg + saturation_factor * (r - avg);
        float new_g = avg + saturation_factor * (g - avg);
        float new_b = avg + saturation_factor * (b - avg);

        data[i] = (new_r > 255) ? 255 : ((new_r < 0) ? 0 : new_r);
        data[i + 1] = (new_g > 255) ? 255 : ((new_g < 0) ? 0 : new_g);
        data[i + 2] = (new_b > 255) ? 255 : ((new_b < 0) ? 0 : new_b);
    }
}

//----------------------------------------------------------------------------------------------------

void emboss(unsigned char* data, int width, int height, int channels) {
    const int emboss_intensity = 128; // Adjust this value to change the emboss intensity

    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            int current_index = y * width + x;
            unsigned char current_r = data[current_index * channels];
            unsigned char current_g = data[current_index * channels + 1];
            unsigned char current_b = data[current_index * channels + 2];

            int next_index = (y + 1) * width + (x + 1);
            unsigned char next_r = data[next_index * channels];
            unsigned char next_g = data[next_index * channels + 1];
            unsigned char next_b = data[next_index * channels + 2];

            int emboss_r = current_r - next_r + emboss_intensity;
            int emboss_g = current_g - next_g + emboss_intensity;
            int emboss_b = current_b - next_b + emboss_intensity;

            emboss_r = (emboss_r > 255) ? 255 : ((emboss_r < 0) ? 0 : emboss_r);
            emboss_g = (emboss_g > 255) ? 255 : ((emboss_g < 0) ? 0 : emboss_g);
            emboss_b = (emboss_b > 255) ? 255 : ((emboss_b < 0) ? 0 : emboss_b);

            data[current_index * channels] = (unsigned char)emboss_r;
            data[current_index * channels + 1] = (unsigned char)emboss_g;
            data[current_index * channels + 2] = (unsigned char)emboss_b;
        }
    }
}

//----------------------------------------------------------------------------------------------------

void invert_colors(unsigned char* data, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; i++) {
        data[i] = 255 - data[i];
    }
}

//----------------------------------------------------------------------------------------------------

void infraredFilter(unsigned char* data, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; i += channels) {
        unsigned char r = data[i];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];

        unsigned char infraredR = 255 - r;
        unsigned char infraredG = 255 - g;
        unsigned char infraredB = 255 - b;

        data[i] = infraredR;
        data[i + 1] = infraredG;
        data[i + 2] = infraredB;
    }
}

//----------------------------------------------------------------------------------------------------

void distort(unsigned char* data, int width, int height, int channels) {
    // Iterate over each pixel in the image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate the index of the current pixel in the image data
            int index = (y * width + x) * channels;

            // Retrieve the color channels of the current pixel
            unsigned char* pixel = &data[index];

            // Example distortion: Swap red and blue channels
            unsigned char temp = pixel[0]; // Red channel
            pixel[0] = pixel[2]; // Blue channel
            pixel[2] = temp; // Swap red and blue channels
        }
    }
}

//----------------------------------------------------------------------------------------------------

void noise(unsigned char* data, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; i += channels) {
        unsigned char r = data[i];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];

        int noise = rand() % 51 - 25; // Random noise between -25 and 25

        data[i] = (r + noise > 255) ? 255 : ((r + noise < 0) ? 0 : (r + noise));
        data[i + 1] = (g + noise > 255) ? 255 : ((g + noise < 0) ? 0 : (g + noise));
        data[i + 2] = (b + noise > 255) ? 255 : ((b + noise < 0) ? 0 : (b + noise));
    }
}

//----------------------------------------------------------------------------------------------------

void pixelate(unsigned char* data, int width, int height, int channels) {
    int blockSize = 10; // Default block size

    for (int y = 0; y < height; y += blockSize) {
        for (int x = 0; x < width; x += blockSize) {
            int blockR = 0, blockG = 0, blockB = 0;
            int blockPixels = 0;

            // Calculate the average color values within the block
            for (int offsetY = 0; offsetY < blockSize && y + offsetY < height; offsetY++) {
                for (int offsetX = 0; offsetX < blockSize && x + offsetX < width; offsetX++) {
                    int pixelX = x + offsetX;
                    int pixelY = y + offsetY;
                    int index = (pixelY * width + pixelX) * channels;

                    blockR += data[index];
                    blockG += data[index + 1];
                    blockB += data[index + 2];
                    blockPixels++;
                }
            }

            // Calculate the average color values
            int averageR = blockR / blockPixels;
            int averageG = blockG / blockPixels;
            int averageB = blockB / blockPixels;

            // Set the color values of all pixels in the block to the average color
            for (int offsetY = 0; offsetY < blockSize && y + offsetY < height; offsetY++) {
                for (int offsetX = 0; offsetX < blockSize && x + offsetX < width; offsetX++) {
                    int pixelX = x + offsetX;
                    int pixelY = y + offsetY;
                    int index = (pixelY * width + pixelX) * channels;

                    data[index] = averageR;
                    data[index + 1] = averageG;
                    data[index + 2] = averageB;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void render(unsigned char* data, int width, int height, int channels) {
    int blockSize = 10; // Default block size

    for (int y = 0; y < height; y += blockSize) {
        for (int x = 0; x < width; x += blockSize) {
            // Calculate the average color values within the block
            int blockR = 0, blockG = 0, blockB = 0;
            int blockPixels = 0;

            for (int offsetY = 0; offsetY < blockSize && y + offsetY < height; offsetY++) {
                for (int offsetX = 0; offsetX < blockSize && x + offsetX < width; offsetX++) {
                    int pixelX = x + offsetX;
                    int pixelY = y + offsetY;
                    int index = (pixelY * width + pixelX) * channels;

                    blockR += data[index];
                    blockG += data[index + 1];
                    blockB += data[index + 2];
                    blockPixels++;
                }
            }

            // Calculate the average color values
            int averageR = blockR / blockPixels;
            int averageG = blockG / blockPixels;
            int averageB = blockB / blockPixels;

            // Set the color values of all pixels in the block to their corresponding average color values
            for (int offsetY = 0; offsetY < blockSize && y + offsetY < height; offsetY++) {
                for (int offsetX = 0; offsetX < blockSize && x + offsetX < width; offsetX++) {
                    int pixelX = x + offsetX;
                    int pixelY = y + offsetY;
                    int index = (pixelY * width + pixelX) * channels;

                    data[index] = averageR;
                    data[index + 1] = averageG;
                    data[index + 2] = averageB;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void stylize(unsigned char* data, int width, int height, int channels) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;

            // Apply stylize effect to each pixel
            data[index] = 255 - data[index];          // Invert the red channel
            data[index + 1] = 255 - data[index + 1];  // Invert the green channel
            data[index + 2] = 255 - data[index + 2];  // Invert the blue channel
            // For grayscale images, you can use the following line instead
            unsigned char gray = (data[index] + data[index + 1] + data[index + 2]) / 3;
            data[index] = data[index + 1] = data[index + 2] = 255 - gray;
        }
    }
}

//----------------------------------------------------------------------------------------------------

void sketch(unsigned char* data, int width, int height, int channels) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;

            // Calculate the average value of the neighboring pixels
            unsigned char avg = (data[index] + data[index + channels] + data[index + channels * 2]) / 3;

            // Apply the sketch effect by setting the pixel value to the average
            data[index] = data[index + 1] = data[index + 2] = avg;
        }
    }
}

//----------------------------------------------------------------------------------------------------

void artisticFilter(unsigned char* data, int width, int height, int channels) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;

            // Retrieve the pixel values
            unsigned char r = data[index];
            unsigned char g = data[index + 1];
            unsigned char b = data[index + 2];

            // Apply the artistic effect by manipulating the pixel values
            // You can experiment with different calculations and transformations here
            unsigned char gray = (r + g + b) / 3;  // Convert to grayscale
            data[index] = gray;  // Set the red channel to the grayscale value
            data[index + 1] = gray;  // Set the green channel to the grayscale value
            data[index + 2] = gray;  // Set the blue channel to the grayscale value
        }
    }
}

//----------------------------------------------------------------------------------------------------

void textureFilter(unsigned char* data, int width, int height, int channels) {
    int textureWidth = width;
    int textureHeight = height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;
            int textureX = x % textureWidth;  // Calculate the corresponding texture coordinates
            int textureY = y % textureHeight;

            int textureIndex = (textureY * textureWidth + textureX) * channels;

            // Apply the texture effect by blending the texture color with the original pixel color
            for (int c = 0; c < channels; c++) {
                data[index + c] = (data[index + c] + data[textureIndex + c]) / 2;  // Blend the colors
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void vscoFilter(unsigned char* data, int width, int height, int channels) {
    int textureWidth = width;
    int textureHeight = height;

    // Apply color adjustments
    float saturation = 0.8;   // Example saturation adjustment parameter
    float brightness = 0.2;   // Example brightness adjustment parameter
    float contrast = 1.5;     // Example contrast adjustment parameter

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;
            int textureX = x % textureWidth;
            int textureY = y % textureHeight;

            int textureIndex = (textureY * textureWidth + textureX) * channels;

            // Apply color adjustments
            for (int c = 0; c < channels; c++) {
                // Adjust saturation
                float pixelValue = data[index + c] / 255.0;
                float grayValue = 0.2989 * pixelValue + 0.5870 * pixelValue + 0.1140 * pixelValue;
                float adjustedValue = (1 - saturation) * grayValue + saturation * pixelValue;
                data[index + c] = (unsigned char)(adjustedValue * 255);

                // Adjust brightness
                data[index + c] = (unsigned char)(data[index + c] * brightness);

                // Adjust contrast
                data[index + c] = (unsigned char)((data[index + c] - 127.5) * contrast + 127.5);
            }

            // Apply tone curve adjustments
            // Example: Increase the brightness of mid-tones
            for (int c = 0; c < channels; c++) {
                float pixelValue = data[index + c] / 255.0;
                float adjustedValue = pixelValue * 0.8 + pow(pixelValue, 3) * 0.2;  // Example tone curve adjustment
                data[index + c] = (unsigned char)(adjustedValue * 255);
            }

            // Apply grain effect
            float grainIntensity = 0.1;  // Example grain intensity
            for (int c = 0; c < channels; c++) {
                float randomValue = (float)rand() / RAND_MAX;  // Generate a random value between 0 and 1
                float grainOffset = (randomValue - 0.5) * grainIntensity;
                int noisyValue = data[index + c] + grainOffset * 255;
                data[index + c] = (unsigned char)fmax(0, fmin(255, noisyValue));
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------

void VintageFilter(unsigned char* data, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; i += channels) {
        unsigned char r = data[i];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];

        // Convert RGB to sepia tone
        unsigned char sepiaR = (unsigned char)((r * 0.393) + (g * 0.769) + (b * 0.189));
        unsigned char sepiaG = (unsigned char)((r * 0.349) + (g * 0.686) + (b * 0.168));
        unsigned char sepiaB = (unsigned char)((r * 0.272) + (g * 0.534) + (b * 0.131));

        // Apply vintage effect
        unsigned char vintageR = (sepiaR < 50) ? (unsigned char)(sepiaR * 0.5) : sepiaR;
        unsigned char vintageG = (sepiaG < 50) ? (unsigned char)(sepiaG * 0.5) : sepiaG;
        unsigned char vintageB = (sepiaB < 50) ? (unsigned char)(sepiaB * 0.5) : sepiaB;

        // Update the pixel values with the vintage effect
        data[i] = vintageR;
        data[i + 1] = vintageG;
        data[i + 2] = vintageB;
    }
}

//----------------------------------------------------------------------------------------------------

void RetroFilter(unsigned char* data, int width, int height, int channels) {
    // Apply desaturation effect
    for (int i = 0; i < width * height * channels; i += channels) {
        unsigned char r = data[i];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];

        // Calculate grayscale value
        unsigned char gray = (unsigned char)((r + g + b) / 3);

        // Apply desaturation by setting RGB channels to grayscale value
        data[i] = gray;
        data[i + 1] = gray;
        data[i + 2] = gray;
    }

    // Apply vignetting effect
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    float maxDist = sqrtf(centerX * centerX + centerY * centerY);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * channels;
            float dx = x - centerX;
            float dy = y - centerY;
            float dist = sqrtf(dx * dx + dy * dy);

            // Calculate vignette strength based on distance from the center
            float strength = (maxDist - dist) / maxDist;
            strength = (strength < 0.2f) ? 0.2f : strength; // Minimum strength

            // Apply vignetting by multiplying RGB channels with strength
            data[index] = (unsigned char)(data[index] * strength);
            data[index + 1] = (unsigned char)(data[index + 1] * strength);
            data[index + 2] = (unsigned char)(data[index + 2] * strength);
        }
    }

    // Apply color tone shift
    int redShift = 10;   // Increase red channel
    int greenShift = -5; // Decrease green channel
    int blueShift = -10; // Decrease blue channel

    for (int i = 0; i < width * height * channels; i += channels) {
        // Apply color tone shift to each RGB channel
        data[i] = (unsigned char)(data[i] + redShift);
        data[i + 1] = (unsigned char)(data[i + 1] + greenShift);
        data[i + 2] = (unsigned char)(data[i + 2] + blueShift);
    }
}


//----------------------------------------------------------------------------------------------------

void GrayscaleFilter(unsigned char* data, int width, int height, int channels) {
    for (int i = 0; i < width * height * channels; i += channels) {
        unsigned char r = data[i];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];

        // Calculate grayscale value
        unsigned char gray = (unsigned char)((r + g + b) / 3);

        // Set RGB channels to grayscale value
        data[i] = gray;
        data[i + 1] = gray;
        data[i + 2] = gray;
    }
}


