#include <bits/stdc++.h>
#include <tuple>
#include <vector>

using namespace std;

class Point
{
    public:
    float x, y;
    int OriginIdx;
};

int compareDistances(const void* a, const void* b)
{
    tuple<int, int, float> *p1 = (tuple<int, int, float> *)a, *p2 = (tuple<int, int, float> *)b;
    return get<2>(*p1) > get<2>(*p2)? 1 : -1;
}

int compareX(const void* a, const void* b)
{
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    return p1->x > p2->x? 1 : -1;
}

int compareY(const void* a, const void* b)
{
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    return p1->y > p2->y? 1 : -1;
}

float dist(const Point p1, const Point p2)
{
    return sqrt( (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

float bruteForce(const Point P[], const int n, tuple<int, int, float>& tGlobal, vector<tuple<int, int, float>>& Dsts)
{
    int iBest = 0, jBest = 0;
    float min = FLT_MAX, dst = 0.0;
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            {
            	dst = dist(P[i], P[j]);
            	tuple<int, int, float> t;
            	get<0>(t) = P[i].OriginIdx;
			    get<1>(t) = P[j].OriginIdx;
			    get<2>(t) = dst;
			    Dsts.push_back(t);

            	if (dst < min)
                { 
                	min = dist(P[i], P[j]);
                	iBest = P[i].OriginIdx;
                	jBest = P[j].OriginIdx;
                }
            }

    get<0>(tGlobal) = iBest;
    get<1>(tGlobal) = jBest;
    get<2>(tGlobal) = min;

    return min;
}

float min(const float x, const float y)
{
    return (x < y)? x : y;
}

float stripClosest(Point strip[], int size, float d, tuple<int, int, float>& tGlobal, vector<tuple<int, int, float>>& Dsts)
{
    float min = d, dst = 0.0;; // Initialize the minimum distance as d
    int iBest = 0, jBest = 0;
 
    qsort(strip, size, sizeof(Point), compareY);

    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j)
        {
        	dst = dist(strip[i], strip[j]);
        	tuple<int, int, float> t;
        	get<0>(t) = strip[i].OriginIdx;
		    get<1>(t) = strip[j].OriginIdx;
		    get<2>(t) = dst;
		    Dsts.push_back(t);

        	if (dist(strip[i],strip[j]) < min)
            {
                min = dist(strip[i], strip[j]);
                iBest = strip[i].OriginIdx;
                jBest = strip[j].OriginIdx;
            }
        }

    get<0>(tGlobal) = iBest;
    get<1>(tGlobal) = jBest;
    get<2>(tGlobal) = min;
 
    return min;
}

float closestUtil(Point P[], int n, tuple<int, int, float>& tGlobal, vector<tuple<int, int, float>>& Dsts)
{
    // If there are 2 or 3 points, then use brute force
    if (n <= 3)
        return bruteForce(P, n, tGlobal, Dsts);
 
    // Find the middle point
    int mid = n/2;
    Point midPoint = P[mid];
 
    // Consider the vertical line passing
    // through the middle point calculate
    // the smallest distance dl on left
    // of middle point and dr on right side
    tuple<int, int, float> tl;
    tuple<int, int, float> tr;
    tuple<int, int, float> tStrip;
    tuple<int, int, float>* t;
    float dl = closestUtil(P, mid, tl, Dsts);
    float dr = closestUtil(P + mid, n - mid, tr, Dsts);
 
    // Find the smaller of two distances
    float d = min(dl, dr);
    t = (dl < dr)? &tl : &tr;
    d = get<2>(*t); 
 
    // Build an array strip[] that contains
    // points close (closer than d)
    // to the line passing through the middle point
    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(P[i].x - midPoint.x) < d)
            strip[j] = P[i], j++;
 
    // Find the closest points in strip.
    // Return the minimum of d and closest
    // distance is strip[]
    float minStrip = stripClosest(strip, j, d, tStrip, Dsts);
    float minGlobal;
    if(d < minStrip)
    {
    	minGlobal = d;
    	get<0>(tGlobal) = get<0>(*t);
    	get<1>(tGlobal) = get<1>(*t);
    	get<2>(tGlobal) = get<2>(*t);
    } 
    else
    {
    	minGlobal = minStrip;
    	get<0>(tGlobal) = get<0>(tStrip);
    	get<1>(tGlobal) = get<1>(tStrip);
    	get<2>(tGlobal) = get<2>(tStrip);
    }

    return minGlobal;
}

int main()
{
    // Point P[] = {{1, 3}, {3, 5}, {9, 4}, {1, 49}, {1, 48}};
    vector<tuple<int, int, float>> Dsts;
    // P[0].OriginIdx = 0;
    // P[1].OriginIdx = 1;
    // P[2].OriginIdx = 2;
    // P[3].OriginIdx = 3;
    // P[4].OriginIdx = 4;
    tuple<int, int, float> t;

    int n = 13146;
    Point P[13146];
    float c;
    FILE* f = fopen("Pts.txt", "r");
    if(!f)
    {
        printf("Cant open file\n");
        return -1;
    }
    int count = 0;

    while(count < n)
    {
        fscanf(f, "%f%f%f", &P[count].x, &P[count].y, &c);
        count++;
    }

    for (int i = 0; i < n; ++i)
        P[i].OriginIdx = i;

    bruteForce(P, n, t, Dsts);
    sort(Dsts.begin(), Dsts.end(), [](const tuple<int, int, float>& lhs, const tuple<int, int, float>& rhs) {
      return get<2>(lhs) < get<2>(rhs);});
    for(auto i: Dsts)
    	printf("(%d, %d), dist is %f\n", get<0>(i), get<1>(i), get<2>(i) );
    printf("Closest pair is (%d, %d), dist is %f\n", get<0>(t), get<1>(t), get<2>(t));
    Dsts.clear();
    return 0;
}
