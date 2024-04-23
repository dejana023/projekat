#ifndef IP_C
#define IP_C
#include "ip.hpp"

SC_HAS_PROCESS(Ip);

Ip::Ip(sc_module_name name) :
    sc_module(name),
    ready(1)
    
{
_index.resize(_IndexSize, std::vector<std::vector<num_f>>(
        _IndexSize, std::vector<num_f>(4, 0.0f)));
        interconnect_socket.register_b_transport(this, &Ip::b_transport);
        cout << "IP constructed" << endl;
}
    
    
Ip::~Ip()
{
	SC_REPORT_INFO("Ip", "Destroyed");
}


void Ip::b_transport(pl_t& pl, sc_time& offset)
{
    tlm_command cmd = pl.get_command();
    sc_dt::uint64 addr = pl.get_address();
    unsigned char *buf = pl.get_data_ptr();
    unsigned int len = pl.get_data_length();
    pl.set_response_status(TLM_OK_RESPONSE);

    switch (cmd)
    {
        case TLM_WRITE_COMMAND:
            switch(addr)
            {
                case addr_start:
                    start = toInt(buf);
                    proc();
                    			         //cout << "start IP: " << start << endl;
                    break;
                    
                case addr_iradius:
                    iradius = toInt(buf);
                    				 //cout << "iradius IP: " << iradius << endl;
                    break;	
                case addr_fracr:
                    fracr = toDouble(buf);
                    				 //cout << "fracr IP: " << fracr << endl;
                    break;
                case addr_fracc:
                    fracc = toDouble(buf);
                    				 //cout << "fracc IP: " << fracc << endl;
                    break;
                case addr_spacing:
                    spacing = toDouble(buf);
                    				 //cout << "spacing IP: " << spacing << endl; 
                    break;
                case addr_iy:
                    iy = toInt(buf);
                                                 //cout << "iy IP: " << iy << endl;
                    break;
                case addr_ix:
                    ix = toInt(buf);
                                                 //cout << "ix IP: " << ix << endl;
                    break;
                /*case addr_r:
                    r = toInt(buf);
                                                 // cout << "r IP: " << r << endl;
                    break;
                case addr_c:
                    c = toInt(buf);
                                                 // cout << "c IP: " << c << endl;
                    break;
                case addr_rpos:
                    rpos = toDouble(buf);
                                                 // cout << "rpos IP: " << rpos << endl;
                    break;
                case addr_cpos:
                    cpos = toDouble(buf);
                                                //  cout << "cpos IP: " << cpos << endl;
                    break;
                case addr_rx:
                    rx = toDouble(buf);
                                                //  cout << "rx IP: " << rx << endl;
                    break;
                case addr_cx:
                    cx = toDouble(buf);
                                                //  cout << "cx IP: " << cx << endl;
                    break;*/
                case addr_step:
                    step = toInt(buf);
                                                //  cout << "step IP: " << step << endl;
                    break;
                case addr_cose:
                    _cose = toDouble(buf);
                                               //   cout << "_cose IP: " << _cose << endl;
                    break;
                case addr_sine:
                    _sine = toDouble(buf);
                             			//  cout << "_sine IP: " << _sine << endl;
                    break;
                    
                default:
                    pl.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
                    cout << "Wrong address" << endl;
            }
            break;
                
        case TLM_READ_COMMAND:
            switch(addr)
            {
                case addr_ready:
                    intToUchar(buf, ready);
                    break;
                default:
                    pl.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
            }
            break;
            
        default:
            pl.set_response_status(TLM_COMMAND_ERROR_RESPONSE);
            cout << "Wrong command" << endl;
    }
    offset += sc_time(10, SC_NS);
}



void Ip::proc() {

    //std::vector<num_f> _lookup2(40);
    //for (int n=0;n<40;n++)
          //_lookup2[n]=exp(-((num_f)(n+0.5))/8.0);
          
          _lookup2 = {.939411163330078125, .829029083251953125, .7316131591796875, .6456451416015625, .569782257080078125, .50283050537109375, .443744659423828125, .391605377197265625, .34558868408203125, .304981231689453125, .269145965576171875, .237518310546875, .2096099853515625, .184978485107421875, .163242340087890625, .144062042236328125, .127132415771484375, .112194061279296875, .099010467529296875, .087375640869140625, .07711029052734375, .068050384521484375, .06005096435546875, .052997589111328125, .0467681884765625, .041271209716796875, .0364227294921875, .03214263916015625, .0283660888671875, .02503204345703125, .022090911865234375, .01949310302734375, .01720428466796875, .0151824951171875, .013397216796875, .011821746826171875, .010433197021484375, .00920867919921875, .00812530517578125, .007171630859375} ;
          
          // Initialize _index array
  for (int i = 0; i < _IndexSize; i++) {
    for (int j = 0; j < _IndexSize; j++) {
      for (int k = 0; k < 4; k++)
        _index[i][j][k] = 0.0;
    }
  }


        // Ispisivanje sadržaja niza _lookup2
        for (const auto& value : _lookup2) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        
      
    if (start == 1 && ready == 1)
    {
        cout << "Uslo u start = 1 i ready = 1" << endl;
        ready = 0;
        offset += sc_time(DELAY, SC_NS);
    }
    
    else if (start == 0 && ready == 0)
    {
        cout << "Processing started" << endl;

        
        // Examine all points from the gradient image that could lie within the
  // _index square.
  for (int i = -iradius; i <= iradius; i++)
    for (int j = -iradius; j <= iradius; j++) {
    
     /* static int counterfor;
      counterfor++;
      cout << "Uslo u for petlju " << counterfor << " puta" << endl;*/
      // Rotate sample offset to make it relative to key orientation.
      // Uses (x,y) coords.  Also, make subpixel correction as later image
      // offset must be an integer.  Divide by spacing to put in _index units.
      rpos = (step*(_cose * i + _sine * j) - fracr) / spacing;
      cpos = (step*(- _sine * i + _cose * j) - fracc) / spacing;
      
     // cout << "rpos: " << rpos << ", cpos: " << cpos << endl;
      
    /*              static int counter;
        counter ++;
        cout << "uslo u for petlju: " << counter << " puta, " << "rpos: " << rpos << "cpos: " << cpos << endl;*/
      
      // Compute location of sample in terms of real-valued _index array
      // coordinates.  Subtract 0.5 so that rx of 1.0 means to put full
      // weight on _index[1] (e.g., when rpos is 0 and _IndexSize is 3.
      rx = rpos + _IndexSize / 2.0 - 0.5;
      cx = cpos + _IndexSize / 2.0 - 0.5;
      
     // cout << "rx: " << rx << ", cx: " << cx << endl;

      // Test whether this sample falls within boundary of _index patch
      if (rx > -1.0 && rx < (double) _IndexSize  &&
          cx > -1.0 && cx < (double) _IndexSize) {
          
          int r = iy + i*step;
          int c = ix + j*step;
          
          //cout << "r: " << r << ", c: " << c << endl;
          
          AddSample(r, c, rpos, cpos, rx, cx, step);
      }
        
        
    }    cout << "Sadržaj _index niza:" << endl;
    for (int i = 0; i < _IndexSize; ++i) {
        for (int j = 0; j < _IndexSize; ++j) {
            for (int k = 0; k < 4; ++k) {
                cout << "_index[" << i << "][" << j << "][" << k << "]: " << _index[i][j][k] << endl;
            }
        }
    
}

mem.clear();

num_f* index1D = new num_f[_IndexSize * _IndexSize * 4];
        
        //index_1d = new num_f[_IndexSize * _IndexSize * 4];
                  int index1D_index = 0;
                  for (int i = 0; i < _IndexSize; i++)
                  {
                      for (int j = 0; j < _IndexSize; j++)
                      {
                          for (int k = 0; k < _IndexSize; k++) {
                              index1D[index1D_index++] = static_cast<num_f>(_index[i][j][k]);
                          }
                      }
                  }
                  
                  for (long unsigned int i = 0; i < _IndexSize*_IndexSize*4; ++i)
                  {
                      mem.push_back(index1D[i]);
                  }
                  
                  
                  
          //ISPIS PIXELSA    
              /*for (int y = 0; y < _width; ++y)
    	      {
                  for (int x = 0; x < _height; ++x)
                  {
                      std::cout << static_cast<int>(pixels1D[y * _height + x]) << " ";
                  }
                  std::cout << std::endl;
              }  
              
              std::cout << "///////////////////////////////////////////////////////////" << endl;*/
                
                  for (int i = 0; i < _IndexSize * _IndexSize * 4; i++)
                  {
                      pl_t pl;
    		      offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
    		      unsigned char* buf;
    		      buf = (unsigned char*)&mem[i];
   		      pl.set_address(addr_index1+i);
  		      pl.set_data_length(1);
  		      pl.set_data_ptr(buf);
   		      pl.set_command(tlm::TLM_WRITE_COMMAND);
  		      pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
  		      mem_socket->b_transport(pl, offset);
                  
                  }
                  
                  
    
    
          //  cout << "Upis iz IP-a u memoriju zavrsen" << endl;
        ready = 1;   
    
}

}

void Ip::AddSample(num_i r, num_i c, num_f rpos,
                     num_f cpos, num_f rx, num_f cx, num_i step) {
        num_f weight;
        num_f dx, dy;
        
       /* static int counter1;
        counter1++;
        //cout << "uslo u add sample pre ifa " << counter1 << " puta" << endl;*/

        if (r < 1+step || r >= _height-1-step || c < 1+step || c >= _width-1-step ) return;
        /*static int counter;
        counter ++;
        cout << "uslo u add sample: " << counter << " puta" << endl;*/
        //cout << "uslo u add sample: " << counter << " puta, " << "rpos: " << rpos << "cpos: " << cpos << endl;
    vector<num_f> pixels1D;
         
         for (int w = 0; w < _width; w++)
       {
           offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
           for (int h = 0; h < _height; h++)
           {
               offset += sc_core::sc_time(DELAY, sc_core::SC_NS);
               pixels1D.push_back( read_mem(addr_Pixels1 + (w * _height + h)));
           }
       }
       
       
       /*for (int w = 0; w < _width; w++) {
       for (int h = 0; h < _height; h++) {
               std::cout << static_cast<num_f>((pixels1D[w * _height + h])) << " ";
       }
           }*/
           
        num_f** _Pixels = new num_f*[_width];
        for (int i = 0; i < _width; i++) {
            _Pixels[i] = new num_f[_height];
        }
    
    
        int pixels1D_index2 = 0;
        for (int w = 0; w < _width; w++) {
            for (int h = 0; h < _height; h++) {
                _Pixels[w][h] = static_cast<num_f>(pixels1D[pixels1D_index2++]);
            }
        }  
    
        /*for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            std::cout << _Pixels[i][j] << " ";
        }
        std::cout << std::endl;
        }*/
        

        
        
         
       /* // Ispisivanje sadržaja niza _lookup2
        for (const auto& value : _lookup2) {
            std::cout << value << " ";
        }
        std::cout << std::endl;*/
        
        weight = _lookup2[num_i(rpos * rpos + cpos * cpos)];
    
        //cout << "weight: " << weight << endl;
  
        num_f dxx, dyy;

        dxx = weight*get_wavelet2(_Pixels, c, r, step);
        //cout << "dxx: " << dxx << endl;
        dyy = weight*get_wavelet1(_Pixels, c, r, step);
             //   cout << "dyy: " << dyy << endl;
        dx = _cose*dxx + _sine*dyy;
             //   cout << "dx: " << dx << endl;
        dy = _sine*dxx - _cose*dyy;
            //   cout << "dy: " << dy << endl;

        PlaceInIndex(dx, (dx<0?0:1), dy, (dy<0?2:3), rx, cx);
 
        //delete[] index_1d;
        //delete[] pixels1D;
        for (int i = 0; i < _width; i++) {
            delete[] _Pixels[i];
        }
        delete[] _Pixels;
    
    

   
    
}
    



void Ip::PlaceInIndex(num_f mag1, num_i ori1, num_f mag2, num_i ori2, num_f rx, num_f cx) {
    
  //  cout << "Uslo u PlaceInIndex" << endl;
    
    num_i ri = std::max(0, std::min(static_cast<int>(_IndexSize - 1), static_cast<int>(rx)));
    num_i ci = std::max(0, std::min(static_cast<int>(_IndexSize - 1), static_cast<int>(cx)));
  
    num_f rfrac = rx - ri;
    num_f cfrac = cx - ci;
  
    rfrac = std::max(0.0f, std::min(float(rfrac), 1.0f));
    cfrac = std::max(0.0f, std::min(float(cfrac), 1.0f));
  
    num_f rweight1 = mag1 * (1.0 - rfrac);
    num_f rweight2 = mag2 * (1.0 - rfrac);
    num_f cweight1 = rweight1 * (1.0 - cfrac);
    num_f cweight2 = rweight2 * (1.0 - cfrac);
    
  // cout << "ri: " << ri << endl;
  //  cout << "ci: " << ci << endl;
  //  cout << "rweight1: " << rweight1 << endl;
  //  cout << "_IndexSize: " << _IndexSize << endl;
    
  //  cout << "Ispred ifova u PlaceInIndex" << endl;
    

    if (ri >= 0 && ri < _IndexSize && ci >= 0 && ci < _IndexSize) {
   //         cout << "Uslo u prvi if" << endl;
   //          cout << "Pristupam _index[" << ri << "][" << ci << "][" << ori1 << "]" << endl;
  //  cout << "Pristupam _index[" << ri << "][" << ci << "][" << ori2 << "]" << endl;
        _index[ri][ci][ori1] += cweight1;
        _index[ri][ci][ori2] += cweight2;
    } else {
   //          cout << "Neuspesan pristup _index vektoru!" << endl;
   //     cout << "ri: " << ri << ", ci: " << ci << endl;
   //     cout << "_IndexSize: " << _IndexSize << endl;
    }

    if (ci + 1 < _IndexSize) {
        _index[ri][ci + 1][ori1] += rweight1 * cfrac;
        _index[ri][ci + 1][ori2] += rweight2 * cfrac;
    }

    if (ri + 1 < _IndexSize) {
        _index[ri + 1][ci][ori1] += mag1 * rfrac * (1.0 - cfrac);
        _index[ri + 1][ci][ori2] += mag2 * rfrac * (1.0 - cfrac);
    }
    
   // cout << "Doslo na kraj PlaceInIndex" << endl;
   
  /* cout << "Sadržaj _index niza:" << endl;
    for (int i = 0; i < _IndexSize; ++i) {
        for (int j = 0; j < _IndexSize; ++j) {
            for (int k = 0; k < 4; ++k) {
                cout << "_index[" << i << "][" << j << "][" << k << "]: " << _index[i][j][k] << endl;
            }
        }
  }*/
}



//VIDI JE L DOBRA OVA FUNKCIJA
void Ip::write_mem(sc_uint<64> addr, num_f val)
{
    pl_t pl;
    unsigned char buf[6];
    doubleToUchar(buf,val);
    pl.set_address(addr);
    pl.set_data_length(1);
    pl.set_data_ptr(buf);
    pl.set_command(tlm::TLM_WRITE_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    mem_socket->b_transport(pl, offset);
}


/*void Ip::read_mem(sc_uint<64> addr)
{
    //for (int i=0; i < _width*_height; i++) {
    pl_t pl;
    unsigned char* buf = new unsigned char;
    pl.set_address(addr);
    pl.set_data_length(1);
    pl.set_data_ptr(buf);
    pl.set_command(tlm::TLM_READ_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    mem_socket->b_transport(pl, offset);
    //mem.push_back(num_f(*buf));
    //mem[i] = ((num_f*)buf)[i];
    //cout << mem[i] << endl;
    //delete buf;
    //}
}*/

/*num_f Ip::read_mem(sc_uint<64> addr)
{
    pl_t pl;
    num_f result;
    unsigned char* buf = reinterpret_cast<unsigned char*>(&result);
    pl.set_address(addr);
    pl.set_data_length(6); // Postavljamo dužinu podataka na dužinu tipa num_f
    pl.set_data_ptr(buf); // Postavljamo pokazivač na bafer za čitanje
    pl.set_command(tlm::TLM_READ_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    mem_socket->b_transport(pl, offset);
    cout << result << endl;
    return result;
}*/

//OVAJ RADI S NULAMA
/*num_f Ip::read_mem(sc_uint<64> addr)
{
    pl_t pl;
    num_f result;
    pl.set_address(addr);
    pl.set_data_length(1);
    pl.set_data_ptr(reinterpret_cast<unsigned char*>(&result));
    pl.set_command(tlm::TLM_READ_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    mem_socket->b_transport(pl, offset);
    //cout << result.to_double() << endl;
    return result;
}*/

num_f Ip::read_mem(sc_dt::sc_uint<64> addr)
{
	pl_t pl;
	sc_dt::sc_int <64> val;
	unsigned char buf[6];
	pl.set_address(addr);
	pl.set_data_length(6); 
	pl.set_data_ptr(buf);
	pl.set_command( tlm::TLM_READ_COMMAND );
	pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
	mem_socket->b_transport(pl,offset);
	
	num_f mega = toNum_f(buf);
	
	//cout << "buf iz ip-a: " << mega << endl;

	return toNum_f(buf);
}

/*num_f Ip::read_mem(sc_uint<64> addr)
{
    pl_t pl;
    unsigned char buf[sizeof(num_f)]; // Napravite bafer odgovarajuće veličine
    pl.set_address(addr);
    pl.set_data_length(sizeof(num_f)); // Postavite dužinu podataka na veličinu num_f
    pl.set_data_ptr(buf);
    pl.set_command(tlm::TLM_READ_COMMAND);
    pl.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    mem_socket->b_transport(pl, offset);
    return toDouble(buf); // Konvertujte podatke iz bafera u num_f pomoću vaše funkcije
}*/




/*num_f Ip::read_mem(sc_uint<64> addr)
{
    pl_t pl;
    num_f result;
    unsigned char* buf = new unsigned char[sizeof(result)];
    pl.set_address(addr);
    pl.set_data_length(sizeof(result));
    pl.set_data_ptr(buf);
    pl.set_command(tlm::TLM_READ_COMMAND);
    
    // Izvršavamo TLM transakciju
    unsigned int status = mem_socket->transport_dbg(pl);
    
    //cout << "Reading from address: " << addr << endl;
    //cout << "Data length: " << pl.get_data_length() << endl;
    //cout << "Response status: " << status << endl;

    if (status == 1) { // Ako je status 1, tada je odgovor OK
        // Kopiramo podatke iz bafera u rezultujući tip
        result = toNum_f(buf);
        //cout << "Read result: " << result << endl;
    } else {
        //cout << "Error reading memory at address " << addr << endl;
    }
    
    // Oslobađamo alociranu memoriju
    delete[] buf;
    
    return result;
}*/










#endif // IP_C
