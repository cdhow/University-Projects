import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { Routes, RouterModule } from '@angular/router';

import { IonicModule } from '@ionic/angular';

import { ResultsPage } from './results.page';
import  { BarGraphComponent } from 'src/app/graphs/bar-graph/bar-graph.component';
import { LineGraphComponent } from 'src/app/graphs/line-graph/line-graph.component';


const routes: Routes = [
  {
    path: '',
    component: ResultsPage
  }
];

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    RouterModule.forChild(routes)
  ],
  declarations: [ResultsPage, BarGraphComponent, LineGraphComponent]
})
export class ResultsPageModule {}
