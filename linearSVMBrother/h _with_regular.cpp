



//mine
float LinearSVM::loss_one_image(const std::vector<int> &image, const int &y){

    assert(image.size() == 3073);

    std::vector<float> scores(10, 0);

    // Compute scores
    // scores = W*x
    for(int c=0; c<C; ++c){
        for(int d=0; d<D; ++d){
            scores[c] += W(c,d)*image[d];
        }
    }

    // Compute loss
    float loss = 0;
    float counter =0;
     std::vector<float> margin(10,0); //vector for loss functions (should be 10 in order to simplify the for loo for dw)   

//compute loss
    for (int j=0; j<C; ++j)
    {
        if(j==y) continue;
         margin[j] =std::max(0.0, scores[j] - scores[y] + 1);
        if (margin[j] > 0.0f){
            loss += margin[j];
            counter+=1;

        }
    }

//compute gradient

            // Compute gradient
            for (int x=0; x<dW.xSize(); ++x){
            for (int y=0; y< dW.ySize(); ++y){
                if(y==x){
                dW(x,y)+=-1*counter*image[dy] + lambda* W(x,y);
                }else if(y!=x){
                 dW(x,y)+=(margin[x]>0) *image[dy] + lambda * W(x,y);

                }
            }



    return loss;
}
