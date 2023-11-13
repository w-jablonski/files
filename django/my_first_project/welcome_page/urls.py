
from django.urls import path
from welcome_page import views

urlpatterns = [
    path("", views.home, name='home'),
]
