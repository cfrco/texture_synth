"""Texture Synthesis
Anthor : catcfrco (z82206.cat[at]gmail.com)
"""
import Image
import numpy as np
import ctypes
from scipy import mgrid,ndimage
    
synth = ctypes.CDLL("./libsynth.so")

def gauss_kern(size, sizey=None):
    """ Returns a normalized 2D gauss kernel array for convolutions """
    size = int(size)
    if not sizey:
        sizey = size
    else:
        sizey = int(sizey)
    x, y = mgrid[-size:size+1, -sizey:sizey+1]
    g = np.exp(-(x**2/float(size)+y**2/float(sizey)))
    return g / g.max()

def gauss_kern2(size, sizey=None):
    """ Returns a normalized 2D gauss kernel array for convolutions """
    size = int(size)
    if not sizey:
        sizey = size
    else:
        sizey = int(sizey)
    x, y = mgrid[-size:size+1, -sizey:sizey+1]
    g = np.exp(-(x**2/float(size)+y**2/float(sizey)))
    return g / g.sum()

def arr2rgb(imarr):
    r = imarr[:,:,0].astype(ctypes.c_double)
    g = imarr[:,:,1].astype(ctypes.c_double)
    b = imarr[:,:,2].astype(ctypes.c_double)

    rp = r.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    gp = g.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    bp = b.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

    return (r,g,b,rp,gp,bp)

def allocrgb(shape):
    r = np.ndarray(shape,dtype=ctypes.c_double)
    g = np.ndarray(shape,dtype=ctypes.c_double)
    b = np.ndarray(shape,dtype=ctypes.c_double)

    rp = r.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    gp = g.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    bp = b.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

    return (r,g,b,rp,gp,bp)

mods = {
    "best"     : 0,
    "random"   : 1,
    "counting" : 2,
}
def texture_synth(sample,out_shape,wsize,psize,mod="random"):
    if not mod in mods :
        return None

    r,g,b,rp,gp,bp = arr2rgb(sample)
    nr,ng,nb,nrp,ngp,nbp = allocrgb(out_shape)
    
    gauss = gauss_kern((wsize-1)/2).astype(ctypes.c_double)
    pgauss = gauss.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    
    synth.synth(rp,gp,bp,a.shape[0],a.shape[1],nrp,ngp,nbp,
                out_shape[0],out_shape[1],wsize,psize,pgauss,mods[mod])
    
    out = np.ndarray((out_shape[0],out_shape[1],3),dtype=np.uint8)
    out[:,:,0] = nr.astype(np.uint8)
    out[:,:,1] = ng.astype(np.uint8)
    out[:,:,2] = nb.astype(np.uint8)

    return out

def opensample(filename):
    im = Image.open(filename)

    if im.mode != "RGB":
        im = im.convert("RGB")

    return np.asarray(im)

def pyramid(arr,level):
    pyramids = [arr]
    for i in range(1,level+1):
        pyramids += [cpyramid(pyramids[i-1])]

    return pyramids

def cpyramid(arr,wsize=5):
    r,g,b,rp,gp,bp = arr2rgb(arr)
    out_shape = (arr.shape[0]/2,arr.shape[1]/2)
    nr,ng,nb,nrp,ngp,nbp = allocrgb(out_shape)
    
    gauss = gauss_kern2((wsize-1)/2).astype(ctypes.c_double)
    pgauss = gauss.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    
    synth._pyramid(rp,gp,bp,arr.shape[0],arr.shape[1],wsize,pgauss,
                   nrp,ngp,nbp)
    
    out = np.ndarray((out_shape[0],out_shape[1],3),dtype=np.uint8)
    out[:,:,0] = nr.astype(np.uint8)
    out[:,:,1] = ng.astype(np.uint8)
    out[:,:,2] = nb.astype(np.uint8)

    return out

def texture_synth_pyramid(arr,out_shape,wsize,psize,level):
    r,g,b,rp,gp,bp = arr2rgb(arr)
    nr,ng,nb,nrp,ngp,nbp = allocrgb(out_shape)
    
    gauss = gauss_kern((wsize-1)/2).astype(ctypes.c_double)
    pgauss = gauss.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    
    synth.synth_pyramid(rp,gp,bp,a.shape[0],a.shape[1],nrp,ngp,nbp,
                        out_shape[0],out_shape[1],wsize,psize,pgauss,level)
    
    out = np.ndarray((out_shape[0],out_shape[1],3),dtype=np.uint8)
    out[:,:,0] = nr.astype(np.uint8)
    out[:,:,1] = ng.astype(np.uint8)
    out[:,:,2] = nb.astype(np.uint8)

    return out

if __name__ == "__main__":
    a = opensample("D1.jpg")
    #res = texture_synth_pyramid(a,(200,200),3,3,3)
    #Image.fromarray(res).show()

    res = texture_synth(a,(100,100),17,3)
    Image.fromarray(res).show()

    """
    res = texture_synth(a,(200,200),3,3)
    Image.fromarray(res).save("rand_out3x3_1.jpg",quality=100)
    print "done"
    res = texture_synth(a,(200,200),3,3)
    Image.fromarray(res).save("rand_out3x3_2.jpg",quality=100)
    print "done"
    res = texture_synth(a,(200,200),3,3)
    Image.fromarray(res).save("rand_out3x3_3.jpg",quality=100)
    print "done"
    """
