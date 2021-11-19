class Osc {
  private:
    float *table; // ftable
    float phase; // phase offset
    float ndx; // ndx
    int length; // ftable length
    int vecsize; // vector size
    float rate; // sampling rate
    float *output // output audio block
  public:
      Osc(float *tab,float ph=0.f, int len=def_len,
          int vsize=def_vsize, int sr=def_sr)
        :table(tab), phase(ph), length(len),
        vecsize(vsize), ndx(0.f), rate =sr,
        output(new float[vecsize] {};

      ~Osc() {delete [] output; }
      float* Proc(float amp, float freq);
};

float* Osc::Proc(float amp, float freq) {
  float incr = freq*length/rate;
  for(int i=0;i < vecsize; i++) {
    output[i] = amp*table[(int)ndx];
    ndx += incr;
    while(ndx >= length) ndx -= length;
    while(ndx <0) ndx += length;
  }
  return output;
}

    
