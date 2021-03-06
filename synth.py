"""Texture Synthesis
Anthor : catcfrco (z82206.cat[at]gmail.com)
"""
import Image
import numpy as np
import ctypes

synth = ctypes.CDLL("./libsynth.so")

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
def texture_synth(sample,out_shape,wsize,psize,err=0.1,mod="random"):
    if not mod in mods :
        return None

    r,g,b,rp,gp,bp = arr2rgb(sample)
    nr,ng,nb,nrp,ngp,nbp = allocrgb(out_shape)
    
    synth.synth(rp,gp,bp,sample.shape[0],sample.shape[1],nrp,ngp,nbp,
                out_shape[0],out_shape[1],wsize,psize,ctypes.c_double(err),mods[mod])
    
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
    
    synth._pyramid(rp,gp,bp,arr.shape[0],arr.shape[1],wsize,nrp,ngp,nbp)
    
    out = np.ndarray((out_shape[0],out_shape[1],3),dtype=np.uint8)
    out[:,:,0] = nr.astype(np.uint8)
    out[:,:,1] = ng.astype(np.uint8)
    out[:,:,2] = nb.astype(np.uint8)

    return out

def texture_synth_pyramid(sample,out_shape,wsize,psize,level,err=0.2):
    r,g,b,rp,gp,bp = arr2rgb(sample)
    nr,ng,nb,nrp,ngp,nbp = allocrgb(out_shape)
    
    synth.synth_pyramid(rp,gp,bp,sample.shape[0],sample.shape[1],nrp,ngp,nbp,
                        out_shape[0],out_shape[1],wsize,psize,ctypes.c_double(err),level)
    
    out = np.ndarray((out_shape[0],out_shape[1],3),dtype=np.uint8)
    out[:,:,0] = nr.astype(np.uint8)
    out[:,:,1] = ng.astype(np.uint8)
    out[:,:,2] = nb.astype(np.uint8)

    return out

