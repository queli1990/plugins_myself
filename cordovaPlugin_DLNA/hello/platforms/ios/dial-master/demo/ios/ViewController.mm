//
//  ViewController.m
//  dlnaclient_ios_demo2
//
//  Created by louis.chu on 11/9/16.
//  Copyright © 2016 louis.chu. All rights reserved.
//

#import "ViewController.h"
#import <discovery.h>
#import <media-renderer.h>
#import <Foundation/Foundation.h>

using namespace dlna;

typedef Service::Result Result;

class MyDiscovery;

@interface ViewController () {
@private
  bool          m_is_scan;
  MyDiscovery*  m_discovery;
  int           m_volume;
}
  @property (weak, nonatomic) IBOutlet UIButton*    scan_btn;
  @property (weak, nonatomic) IBOutlet UITextView*  test0;
  @property (weak, nonatomic) IBOutlet UITextView*  test1;
@end

class MyDiscovery: public Discovery, public Discovery::Delegate {
public:
  MyDiscovery(ViewController* ctr): m_ctr(ctr) {
    set_delegate(this);
  }
  
  virtual void discovery_change(Discovery* dis) {
    
    vector<Service*> ls = this->service( MEDIA_RENDERER );
    NSString* str = [NSString string];
    
    for ( int i = 0; i < ls.size(); i++ ) {
      str = [str stringByAppendingFormat:
             @"friendly_name:%s, host:%s, port:%d\n",
             ls[i]->get_friendly_name().c_str(),
             ls[i]->get_hostname().c_str(),
             ls[i]->get_port()
             ];
    }
    
    dispatch_async(dispatch_get_main_queue(), ^(){
      [m_ctr.test0 setText:str];
    });
  }
  
  virtual void discovery_error(Discovery* dis, const string& err) {
    
  }
  
private:
  ViewController* m_ctr;
};

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  // Do any additional setup after loading the view, typically from a nib.
  m_is_scan = false;
  m_discovery = new MyDiscovery(self);
  m_volume = 10;
}

- (IBAction)scan:(id)sender {
  m_is_scan = !m_is_scan;
  if ( m_is_scan ) {
    m_discovery->scan( Discovery::MEDIA_RENDERER );
    [self.scan_btn setTitle:@"STOP" forState:UIControlStateNormal];
    [self.test0 setText:@"Scaning..."];
  } else {
    m_discovery->stop();
    [self.scan_btn setTitle:@"SCAN" forState:UIControlStateNormal];
  }
}

#define CALL(exec) \
vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER ); \
for (int i = 0; i < ls.size(); i++) { \
  MediaRenderer* mr = ls[i]->as_media_renderer(); \
  if ( mr ) { \
    exec; \
  } \
}


- (IBAction)start:(id)sender {
  
  CALL({
    
  mr->set_av_transport_uri("http://huace.cdn.ottcloud.tv/huace/videos/dst/2016/08/14461-ai-de-zhui-zong-01-ji_201608180551/14461-ai-de-zhui-zong-01-ji.m3u8");
    
  });
}

- (IBAction)stop:(id)sender {
  CALL({
    mr->stop();
  });
}

- (IBAction)play:(id)sender {
  CALL({
    mr->play();
  });
}

- (IBAction)pause:(id)sender {
  CALL({
    mr->pause();
  });
}

- (IBAction)seek:(id)sender {
  CALL({
    mr->seek(1000);
  });
}

- (IBAction)volume0:(id)sender {
  m_volume--;
  CALL({
    mr->set_volume(m_volume);
  });
}

- (IBAction)volume1:(id)sender {
  m_volume++;
  CALL({
    mr->set_volume(m_volume);
  });
}

- (IBAction)get_media_info:(id)sender {
  
  vector<Service*> ls = m_discovery->service( Discovery::MEDIA_RENDERER );
  NSString* data = [NSString string];
  
  for (int i = 0; i < ls.size(); i++) {
    MediaRenderer* mr = ls[i]->as_media_renderer();
    if ( mr ) {
      // Result res1 = m_discovery->mr()->get_media_info();
      Result res2 = mr->get_position_info();
      
      if ( res2.code == 0 ) {
        data = [data stringByAppendingFormat:@"##MediaDuration: %d, RelTime: %d, TrackURI: %s",
                // res1.data["MediaDuration"].GetInt(),
                res2.data["TrackDuration"].GetInt(),
                res2.data["RelTime"].GetInt(),
                res2.data["TrackURI"].GetString()
        ];
      }
    }
  }
  
  [self.test1 setText:data];
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  delete m_discovery;
  // Dispose of any resources that can be recreated.
}


@end
