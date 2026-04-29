#ifndef MODEL_H
#define MODEL_H

// SVM Model for Hand Gesture Recognition
// Features: S1-S5 (Flex), S6-S10 (FSR)
// Accuracy: 100%

namespace SVMModel {
    const int num_features = 10;
    const int num_classes = 6;

    // Normalization Parameters
    float means[] = {652.871920f, 766.021077f, 751.630350f, 631.911479f, 609.216926f, 2161.792477f, 1106.924125f, 9.718547f, 954.112192f, 802.620947f};
    float scales[] = {115.482918f, 78.473524f, 97.654139f, 176.490917f, 89.600243f, 1433.680503f, 1338.789054f, 30.357883f, 1360.796843f, 1475.017415f};

    // SVM Weights (One-Vs-Rest)
    float weights[] = {
        0.180571f, 0.446869f, 0.697551f, 0.082725f, 0.505704f, -0.627798f, 0.040179f, -0.222955f, -0.198135f, -0.323541f,
        -1.748057f, -1.301389f, -1.615234f, -1.841246f, -1.339178f, -0.871783f, 0.098492f, 1.213797f, 0.283187f, -0.052601f,
        0.419013f, 0.323719f, 0.528659f, -0.239339f, 0.117185f, 0.231998f, 2.039757f, -0.311756f, -0.702741f, -0.544153f,
        0.274351f, 0.282928f, -0.211479f, 0.612217f, -0.184852f, 0.176461f, -0.835153f, -0.320134f, -0.701193f, -0.544111f,
        0.316867f, -1.074092f, 0.772591f, 0.626243f, -0.121303f, 0.563044f, -0.825833f, -0.320119f, 0.730302f, -0.544114f,
        0.177435f, 0.169135f, -0.131920f, 0.468897f, 0.201717f, 1.077209f, -0.835165f, -0.320120f, -0.701183f, 2.441618f
    };
    float intercepts[] = {0.339908f, -0.490214f, -0.999718f, -1.000329f, -0.999775f, -1.000109f};

    int predict(int* raw_sensors) {
        float scaled[num_features];
        for (int i = 0; i < num_features; i++) {
            scaled[i] = ((float)raw_sensors[i] - means[i]) / scales[i];
        }

        float max_score = -1e10;
        int best_class = 0;

        for (int i = 0; i < num_classes; i++) {
            float score = intercepts[i];
            for (int f = 0; f < num_features; f++) {
                score += weights[i * num_features + f] * scaled[f];
            }
            if (score > max_score) {
                max_score = score;
                best_class = i;
            }
        }
        return best_class;
    }
}
#endif