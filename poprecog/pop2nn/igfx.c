struct sIImage
{
    BITMAP *bitmap;
    int direction;
    int rangeForw, rangeBack, footRangeForw, footRangeBack;
};    


struct sIImages
{
    struct sIImage iImage[9999];
};
