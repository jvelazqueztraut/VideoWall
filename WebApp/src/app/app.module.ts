import { BrowserModule } from '@angular/platform-browser';
import { NgModule, ErrorHandler } from '@angular/core';
import { IonicApp, IonicModule, IonicErrorHandler } from 'ionic-angular';
import { IonicStorageModule } from '@ionic/storage';
import { HttpModule } from '@angular/http';
import { MyApp } from './app.component';

import { DataBaseService } from './database.service';

import { StatusPage } from '../pages/status/status';
import { ConfigPage } from '../pages/configuration/configuration';
import { ListPage } from '../pages/list/list';
import { ContentPage } from '../pages/content/content';

import { ColorPopover } from '../pages/color/color';

import { StatusBar } from '@ionic-native/status-bar';
import { SplashScreen } from '@ionic-native/splash-screen';

@NgModule({
  declarations: [
    MyApp,
    StatusPage,
    ConfigPage,
    ContentPage,
    ListPage,
    ColorPopover
  ],
  imports: [
    BrowserModule,
    IonicModule.forRoot(MyApp),
    IonicStorageModule.forRoot(),
    HttpModule
  ],
  bootstrap: [IonicApp],
  entryComponents: [
    MyApp,
    StatusPage,
    ConfigPage,
    ContentPage,
    ListPage,
    ColorPopover
  ],
  providers: [
    DataBaseService,
    StatusBar,
    SplashScreen,
    {provide: ErrorHandler, useClass: IonicErrorHandler}
  ]
})
export class AppModule {}
