#!/usr/bin/python
from synth import *

if __name__ == "__main__":
    sample1 = opensample("example/bread.jpg")
    sample2 = opensample("example/cheetah.jpg")
    sample3 = opensample("example/D1.jpg")
    sample4 = opensample("example/metal_small.bmp")
    
    #sample1
    res = texture_synth(sample1,(200,200),3,3)
    Image.fromarray(res).save("example/out/bread_out3x3_1.jpg")
    res = texture_synth(sample1,(200,200),3,3)
    Image.fromarray(res).save("example/out/bread_out3x3_2.jpg")
    res = texture_synth(sample1,(200,200),5,3)
    Image.fromarray(res).save("example/out/bread_out5x5_1.jpg")
    res = texture_synth(sample1,(200,200),11,3)
    Image.fromarray(res).save("example/out/bread_out11x11_1.jpg")
    
    # need many time.
    #res = texture_synth(sample1,(400,400),15,3)
    #Image.fromarray(res).save("example/out/bread_out15x15_1.jpg")

    res = texture_synth_pyramid(sample1,(200,200),3,3,3)
    Image.fromarray(res).save("example/out/bread_out3x3p_1.jpg")
    res = texture_synth_pyramid(sample1,(200,200),5,3,3)
    Image.fromarray(res).save("example/out/bread_out5x5p_1.jpg")

    res = texture_synth(sample1,(200,200),3,3,mod="best")
    Image.fromarray(res).save("example/out/bread_out3x3b_1.jpg")
    res = texture_synth(sample1,(200,200),3,3,mod="best")
    Image.fromarray(res).save("example/out/bread_out3x3b_2.jpg")
    res = texture_synth(sample1,(200,200),3,3,mod="counting")
    Image.fromarray(res).save("example/out/bread_out3x3c_1.jpg")
    res = texture_synth(sample1,(200,200),3,3,mod="counting")
    Image.fromarray(res).save("example/out/bread_out3x3c_2.jpg")
    
    #sample2
    res = texture_synth(sample2,(200,200),3,3)
    Image.fromarray(res).save("example/out/cheetah_out3x3.jpg")
    res = texture_synth(sample2,(200,200),7,3)
    Image.fromarray(res).save("example/out/cheetah_out7x7.jpg")
    res = texture_synth(sample2,(200,200),11,3)
    Image.fromarray(res).save("example/out/cheetah_out11x11.jpg")
    res = texture_synth(sample2,(200,200),13,3)
    Image.fromarray(res).save("example/out/cheetah_out13x13.jpg")

    res = texture_synth_pyramid(sample2,(200,200),3,3,3)
    Image.fromarray(res).save("example/out/cheetah_out3x3p.jpg")
    
    #sample3
    res = texture_synth(sample3,(200,200),3,3)
    Image.fromarray(res).save("example/out/D1_out3x3.jpg")
    res = texture_synth(sample3,(200,200),7,3)
    Image.fromarray(res).save("example/out/D1_out7x7.jpg")
    res = texture_synth(sample3,(200,200),13,3)
    Image.fromarray(res).save("example/out/D1_out13x13.jpg")
    
    res = texture_synth(sample3,(200,200),11,5,mod="best")
    Image.fromarray(res).save("example/out/D1_out11x11b.jpg")
    res = texture_synth_pyramid(sample3,(200,200),3,3,3)
    Image.fromarray(res).save("example/out/D1_out3x3p.jpg")
    
    #sample4
    res = texture_synth(sample4,(200,200),3,3)
    Image.fromarray(res).save("example/out/metal_out3x3.jpg")
    res = texture_synth(sample4,(200,200),7,3)
    Image.fromarray(res).save("example/out/metal_out7x7.jpg")
    res = texture_synth(sample4,(200,200),13,3)
    Image.fromarray(res).save("example/out/metal_out13x13.jpg")

    res = texture_synth_pyramid(sample4,(200,200),3,3,3)
    Image.fromarray(res).save("example/out/metal_out3x3p.jpg")
