
#include <vtkSmartPointer.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkImageViewer.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageReslice.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSliderRepresentation2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkContourFilter.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkFeatureEdges.h>
#include <vtkPolyDataMapper.h>
#include <vtkScalarsToColors.h>
#include <vtkActor.h>
#include <vtkOutlineFilter.h>
#include <vtkCommand.h>
#include <vtkSliderWidget.h>
#include <vtkImageActor.h>
#include <vtkSliderRepresentation2D.h>

#include <vtkStripper.h>

#include <vtkNamedColors.h>
#include <vtkCamera.h>


#include <vtkProperty2D.h>

#include <vtkVersion.h>
#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>


class vtkSliderCallback : public vtkCommand
{
public:
  static vtkSliderCallback *New() 
    {
    return new vtkSliderCallback;
    }
  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {

    vtkSliderWidget *slidrWidgt = reinterpret_cast<vtkSliderWidget*>(caller);
    int value = static_cast<int>(static_cast<vtkSliderRepresentation *>(slidrWidgt->GetRepresentation())->GetValue());
    this->ContourFilter->SetValue(0, value);
    this->ContourFilter->Modified();
    this->ContourFilter->Update();
    this->normalGenerator->Modified();
    this->normalGenerator->Update();
    this->stripr->Modified();
    this->stripr->Update();
    this->mappr->Modified();
    this->mappr->Update();
    this->rendwind->Modified();
    this->rendwind->Render();
    }
    vtkSliderCallback():ContourFilter(0)
    {
    
    }
    vtkContourFilter *ContourFilter;
    vtkPolyDataNormals *normalGenerator;
    vtkStripper *stripr;
    vtkPolyDataMapper *mappr;
    vtkRenderWindow *rendwind;

  
};


int main(int argc, char *argv[])
{
  /* -----Declarations---- */
  int start =  0,end = 256,x,y,z;

   /* Providing the file name for input */
  vtkSmartPointer<vtkStructuredPointsReader> vspr = vtkSmartPointer<vtkStructuredPointsReader>::New();
  

  vtkSmartPointer<vtkStructuredPoints> datsiz;
  vtkContourFilter *vcf = vtkContourFilter::New(); 
  vtkSmartPointer<vtkCleanPolyData> cleanPolyData = vtkSmartPointer<vtkCleanPolyData>::New();
  vtkSmartPointer<vtkPolyDataNormals> normalGenerator = vtkSmartPointer<vtkPolyDataNormals>::New();
  vtkSmartPointer<vtkOutlineFilter> olf = vtkSmartPointer<vtkOutlineFilter>::New();
  vtkSmartPointer<vtkPolyDataMapper> mappr1 = vtkSmartPointer<vtkPolyDataMapper>::New();
  vtkSmartPointer<vtkPolyDataMapper> mappr2 = vtkSmartPointer<vtkPolyDataMapper>::New();
  vtkSmartPointer<vtkActor> actr1 = vtkSmartPointer<vtkActor>::New();
  vtkSmartPointer<vtkActor> actr2 = vtkSmartPointer<vtkActor>::New();
  vtkSmartPointer<vtkRenderer> rend = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> rendwind = vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> rendwindintr = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  
  vtkSmartPointer<vtkSliderRepresentation2D> slidrRep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  vtkSmartPointer<vtkSliderWidget> slidrWidgt = vtkSmartPointer<vtkSliderWidget>::New();
  vtkSmartPointer<vtkSliderCallback> callback = vtkSmartPointer<vtkSliderCallback>::New();

vtkSmartPointer<vtkCamera> cam = vtkSmartPointer<vtkCamera>::New();


  vspr->SetFileName("/Users/advi/Desktop/Data Viz/Data-Viz-Assignment-2/ISO/ironProt.vtk");
  vspr->Update();

  datsiz = vspr->GetOutput();

  x = datsiz->GetDimensions()[0];
  y = datsiz->GetDimensions()[1];
  z = datsiz->GetDimensions()[2];

  vcf->SetInputConnection(vspr->GetOutputPort());
  vcf->SetValue(0, (start + end) /2 );

  cleanPolyData->SetInputConnection(vcf->GetOutputPort());

  normalGenerator->SetInputConnection(vcf->GetOutputPort());
  normalGenerator->SetFeatureAngle(60.0);

  mappr1->SetInputConnection(normalGenerator->GetOutputPort());
  mappr1->ScalarVisibilityOff();

  actr1->SetMapper(mappr1);
  actr1->GetProperty()->SetDiffuseColor(0.68,0.08,0.12);

  olf->SetInputConnection(vspr->GetOutputPort());

  mappr2->SetInputConnection(olf->GetOutputPort());
  
  actr2->SetMapper(mappr2);
  actr2->GetProperty()->SetColor(0.0,0.0,0.0);
  
  cam->SetViewUp (0, 0, -1);
  cam->SetPosition (0.25, 0.75, 0.25);
  cam->SetFocalPoint (0, 0, 0);
  cam->ComputeViewPlaneNormal();
  cam->Azimuth(30.0);
  cam->Elevation(30.0);


  rend->AddActor(actr2);
  rend->AddActor(actr1);

  rend->SetActiveCamera(cam);
  rend->ResetCamera();
  cam->Dolly(1.5);

  rend->SetBackground(1, 1, 1);
  
  rendwind->SetSize(640,480);

  rend->ResetCameraClippingRange();

  rendwind->AddRenderer(rend);

  
  rendwindintr->SetRenderWindow(rendwind);
  
  
  rendwind->Render();

  slidrRep->SetMinimumValue(start);
  slidrRep->SetMaximumValue(end);
  slidrRep->SetValue(5.0);
  
  slidrRep->SetTitleText("SLIDER");
  slidrRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  slidrRep->GetPoint1Coordinate()->SetValue(0.3 ,0.05);
  slidrRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  slidrRep->GetPoint2Coordinate()->SetValue(0.7, 0.05);

  slidrRep->SetSliderLength(0.02);
  slidrRep->SetSliderWidth(0.03);
  slidrRep->SetEndCapLength(0.01);
  slidrRep->SetEndCapWidth(0.03);
  slidrRep->SetTubeWidth(0.005);
  slidrRep->SetLabelFormat("%3.0lf");
  slidrRep->SetTitleHeight(0.02);
  slidrRep->SetLabelHeight(0.02);
  slidrRep->GetSelectedProperty()->SetColor(1,1,0);

  slidrWidgt->SetInteractor(rendwindintr);
  slidrWidgt->SetRepresentation(slidrRep);
  slidrWidgt->KeyPressActivationOff();
  slidrWidgt->SetAnimationModeToAnimate();
  slidrWidgt->EnabledOn();
  callback->ContourFilter = vcf;
  slidrWidgt->AddObserver(vtkCommand::InteractionEvent,callback);
  



  rendwindintr->Initialize();
  rendwindintr->Start();
  
  return EXIT_SUCCESS;


}